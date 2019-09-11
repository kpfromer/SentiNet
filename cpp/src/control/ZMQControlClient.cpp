
#include "control/ZMQControlClient.hpp"

ZMQControlClient::ZMQControlClient(int context_, const std::string& yaml_file) 
  :   context(context_) {
  // By default, no pub or client
  this_publisher = nullptr;
  this_client = nullptr;

  // Name the node
  meta.control_node_name = utils::defaults::DEFAULT_ZMQ_CONTROL_NAME;

  // For meta information, store the number of threadsd possible
  meta.supported_threads = std::thread::hardware_concurrency();
}

bool ZMQControlClient::start(int) { return true; }

bool ZMQControlClient::quit(int) {
  // TODO - make this look prettier
  for (auto const &i : thread_space.subscribers)
    cancel_subscription(i.first);
  for (auto const &i : thread_space.servers)
    terminate_server(i.first);
  for (auto const &i : thread_space.periodic_publishers)
    cancel_periodic_publisher(i.first);
  for (auto const &i : thread_space.periodic_clients)
    cancel_periodic_request(i.first);
  return true;
}

bool ZMQControlClient::initialize_publisher(const std::string& address) {
  using namespace ::utils;
  this_publisher = std::make_unique<::zmq::socket_t>(context, ZMQ_PUB);
  this_publisher->connect(address); // TODO - Add guards that protect from false endpoints. A ping protocol persay
  return true;
}

bool ZMQControlClient::initialize_client() {
  this_client = std::make_unique<::zmq::socket_t>(context, ZMQ_REQ);
  return true;
}

bool ZMQControlClient::publish(const std::string &topic,
                               const std::string &message) {
  if (!this_publisher) {
    LOG_ERROR("No Concurrent Publisher In This Control Thread");
    return false;
  }
  concurrent_publish(std::move(this_publisher), topic, message);
  return true; // TODO change this
}

bool ZMQControlClient::publish(
    const std::string sock_addr, const std::string topic,
    std::function<std::string (void)> get_data_to_publish,
    std::chrono::microseconds period) {

  // If already exists do nothing
  auto &&found = thread_space.periodic_publishers.find(topic);
  if (found != thread_space.periodic_publishers.end()) {
    std::cout << "Topic publisher already exists" << std::endl;
    return false;
  }

  // Store a new socket by key sock_addr
  // val is now a socket_thread_space
  auto &&val =
      create_socket(ZMQ_PUB, thread_space.periodic_publishers, topic);

  // Create a future promise to exit the thread
  auto futureObj = val.exit_signal.get_future();
  auto context = std::make_unique<Publisher_Context>();

  // Set publisher context properties
  context->set_sock_addr(sock_addr);
  context->set_exit_signal(std::move(futureObj));
  context->set_socket(std::move(val.socket));
  context->set_topic(topic);
  context->set_data_request_callback(get_data_to_publish);
  context->set_period(period);

  // Create a new thread - attatched to periodic  publisher
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_publish_thread,
      std::move(context));
  return true;
}

bool ZMQControlClient::cancel_periodic_publisher(const std::string &reference) {
  thread_space.periodic_publishers[reference].exit_signal.set_value();
  thread_space.periodic_publishers[reference].thread->join();
  return true;
}

std::string ZMQControlClient::request(const std::string destination,
                                      const std::string message) {
  if (!this_client) {
    LOG_ERROR("No Concurrent Publisher In This Control Thread");
    return "";
  }
  return concurrent_request(destination, std::move(this_client), message);
}

bool ZMQControlClient::request(
    const std::string destination, const std::string id,
    std::function<std::string (void)> get_data_to_request,
    std::function<void(std::string &)> action_to_recieved_data,
    const std::chrono::microseconds period) {

  // See if element is allready in map
  auto &&found = thread_space.periodic_clients.find(id);
  if (found != thread_space.periodic_clients.end()) {
    LOG_WARN("Destination already exists");
    return false;
  }

  // create and assign the address of val (a socket_thraed_space) to the map
  // val is now a socket_thread_space
  auto &&val =
      create_socket(ZMQ_REQ, thread_space.periodic_clients, id);

  // Create a future promise to exit
  auto &&futureObj = val.exit_signal.get_future();
  auto context = std::make_unique<Requester_Context>();

  context->set_exit_signal(std::move(futureObj));
  context->set_socket(std::move(val.socket));
  context->set_data_request(get_data_to_request);
  context->set_callback(action_to_recieved_data);
  context->set_period(period);

  // Our main thread
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_request_thread, 
      std::move(context));
  return true;
}

bool ZMQControlClient::cancel_periodic_request(const std::string &reference) {
  thread_space.periodic_clients[reference].exit_signal.set_value();
  thread_space.periodic_clients[reference].thread->join();
  return true;
}

bool ZMQControlClient::subscribe(
    const std::string sock_addr, const std::string topic,
    std::function<void(std::string &)> callback) {

  auto &&found = thread_space.subscribers.find(topic);

  if (found != thread_space.subscribers.end()) {
    LOG_WARN("Subscriber Already Exists");
    return false;
  }

  auto &&val = create_socket(ZMQ_SUB, thread_space.subscribers, sock_addr);

  auto &&futureObj = val.exit_signal.get_future();

  auto context = std::make_unique<Subscriber_Context>();
  
  context->set_exit_signal(std::move(futureObj));
  context->set_socket(std::move(val.socket));
  context->set_callback(callback);

  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::subscription_thread, 
      std::move(context));

  return true;
}

bool ZMQControlClient::cancel_subscription(const std::string &reference) {
  thread_space.subscribers[reference].exit_signal.set_value();
  if (thread_space.subscribers[reference].thread->joinable())
    thread_space.subscribers[reference].thread->join();
  std::cout << "made it" << std::endl;
  return true;
}

bool ZMQControlClient::serve(
    const std::string address,
    std::function<std::string(std::string &)> callback) {

  auto &&found = thread_space.servers.find(address);
  if (found != thread_space.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }

  auto &&val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto &&futureObj = val.exit_signal.get_future();
  
  auto context = std::make_unique<Server_Context>();
  
  context->set_exit_signal(std::move(futureObj));
  context->set_socket(std::move(val.socket));
  context->set_callback(callback);

  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::server_thread, 
      std::move(context));
  return true;
}


bool ZMQControlClient::terminate_server(const std::string &reference) {
  thread_space.servers[reference].exit_signal.set_value();
  thread_space.servers[reference].thread->join();
  return true;
}

// void ZMQControlClient::periodic_publish_thread(std::unique_ptr<Publisher_Context> pub_context) {
//   pub_context->enter_thread();
// }
//   //   const std::string sock_addr, std::future<void> exit_signal,
//   //   std::unique_ptr<::zmq::socket_t> socket, std::string topic,
//   //   std::function<std::string &(void)> get_data,
//   //   const std::chrono::microseconds period) {
//   //
//   // std::string temp = sock_addr;
//   // std::cout << sock_addr << std::endl;
//   // socket->bind(sock_addr);
//   // auto start = std::chrono::steady_clock::now();
//   // while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
//   //        std::future_status::timeout) {
//   //   std::cout<<topic<<std::endl;
//   //   start = std::chrono::steady_clock::now();
//   //   s_sendmore(*socket, topic);
//   //   s_send(*socket, get_data());
//   //   auto wait_time = period + start - std::chrono::steady_clock::now();
//   //   if (wait_time > std::chrono::milliseconds::zero())
//   //     std::this_thread::sleep_for(wait_time);
//   // }
//   // std::cout << "Thread end" << std::endl;
//
// void ZMQControlClient::periodic_request_thread(std::unique_ptr<Requester_Context> req_context) {
//   req_context->enter_thread();
// }
//     std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
//     std::string server, std::function<std::string(void)> get_data,
//     std::function<void(const std::string &)> callback,
//     std::chrono::microseconds period) {
//
//   std::cout << "Thread start" << std::endl;
//
//   auto start = std::chrono::steady_clock::now();
//   while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
//          std::future_status::timeout) {
//
//     start = std::chrono::steady_clock::now();
//     socket->connect(server); // TODO Better way of doing this?
//     s_send(*socket, get_data().c_str());
//     std::string response = s_recv(*socket);
//     callback(response);
//     zmq_disconnect(*socket, server.c_str());
//     auto wait_time = period + start - std::chrono::steady_clock::now();
//     if (wait_time > std::chrono::milliseconds::zero())
//       std::this_thread::sleep_for(wait_time);
//   }
//   std::cout << "Thread end" << std::endl;
// }
//
// void ZMQControlClient::subscription_thread(
//     std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
//     std::function<void(const std::string &)> callback) {
//
//   std::cout << "Thread start" << std::endl;
//
//   std::string message;
//   ::zmq::pollitem_t items[] = {
//       {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
//   while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
//          std::future_status::timeout) {
//     zmq::poll(&items[0], 1, 100);
//     if (items[0].revents & ZMQ_POLLIN) {
//       message = s_recv(*socket);
//       message = s_recv(*socket);
//       callback(message);
//     }
//   }
//   std::cout << "Thread end" << std::endl;
// }
//
// void ZMQControlClient::server_thread_1(
//     std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
//     std::function<std::string(const std::string &)> callback) {
//
//   std::cout << "Thread start" << std::endl;
//   ::zmq::pollitem_t items[] = {
//       {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
//   while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
//          std::future_status::timeout) {
//     zmq::poll(&items[0], 1, 100);
//     if (items[0].revents & ZMQ_POLLIN) {
//       std::string request = s_recv(*socket);
//       s_send(*socket, callback(request));
//     }
//   }
//   std::cout << "Thread end" << std::endl;
// }
//
// void ZMQControlClient::server_thread_2(
//     std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
//     const std::function<std::string(void)> callback) {
//   std::cout << "Thread start" << std::endl;
//   ::zmq::pollitem_t items[] = {
//       {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
//   while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
//          std::future_status::timeout) {
//     zmq::poll(&items[0], 1, 100);
//     if (items[0].revents & ZMQ_POLLIN) {
//       s_recv(*socket); // Throw away
//       s_send(*socket, callback());
//     }
//   }
//   std::cout << "Thread end" << std::endl;
// }
//

void ZMQControlClient::concurrent_publish(
    std::unique_ptr<::zmq::socket_t> socket, const std::string &topic,
    const std::string &message) {
  s_sendmore(*socket, topic);
  s_send(*socket, message);
}

std::string
ZMQControlClient::concurrent_request(const std::string &server,
                                     std::unique_ptr<::zmq::socket_t> socket,
                                     const std::string &message) {
  socket->connect(server); // TODO Better way of doing this?
  s_send(*socket, message);
  std::string response = s_recv(*socket);
  return response;
}
