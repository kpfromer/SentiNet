
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

bool ZMQControlClient::start() { return true; }

bool ZMQControlClient::quit() {
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
  this_publisher->bind(address);
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
    std::string sock_addr, std::string topic,
    std::function<std::string &(void)> get_data_to_publish,
    std::chrono::microseconds period) {

  // If already exists do nothing
  auto &&found = thread_space.periodic_publishers.find(sock_addr);
  if (found != thread_space.periodic_publishers.end()) {
    std::cout << "Topic publisher already exists" << std::endl;
    return false;
  }

  // Store a new socket by key sock_addr
  auto &&val =
      create_socket(ZMQ_PUB, thread_space.periodic_publishers, sock_addr);

  // Create a future promise to exit the thread
  auto futureObj = val.exit_signal.get_future();

  // Create a new thread - attatched to periodic  publisher
  // Remember to reconstruct the sock_addr because it's constant and
  // the thread wants mutable
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_publish_thread, sock_addr,
      std::move(futureObj), std::move(val.socket), topic, get_data_to_publish,
      period);
  return true;
}

bool ZMQControlClient::cancel_periodic_publisher(const std::string &reference) {
  thread_space.periodic_publishers[reference]->exit_signal.set_value();
  thread_space.periodic_publishers[reference]->thread->join();
  return true;
}

std::string ZMQControlClient::request(const std::string &destination,
                                      const std::string &message) {
  if (!this_client) {
    LOG_ERROR("No Concurrent Publisher In This Control Thread");
    return "";
  }
  return concurrent_request(destination, std::move(this_client), message);
}

bool ZMQControlClient::request(
    std::string &destination,
    std::function<std::string &(void)> get_data_to_request,
    std::function<void(const std::string &)> action_to_recieved_data,
    const std::chrono::microseconds &period) {

  // See if element is allready in map
  // TODO change destination to a const reference - shouldnt be passing
  // destination
  auto &&found = thread_space.periodic_clients.find(destination);
  if (found != thread_space.periodic_clients.end()) {
    LOG_WARN("Destination already exists");
    return false;
  }

  // create and assign the address of val (a socket_thraed_space) to the map
  auto &&val =
      create_socket(ZMQ_REQ, thread_space.periodic_clients, destination);

  // Create a future promise to exit
  auto &&futureObj = val.exit_signal.get_future();

  // Our main thread
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_request_thread, std::move(futureObj),
      std::move(val.socket), destination, get_data_to_request,
      action_to_recieved_data, period);
  return true;
}

bool ZMQControlClient::cancel_periodic_request(const std::string &reference) {
  thread_space.periodic_clients[reference]->exit_signal.set_value();
  thread_space.periodic_clients[reference]->thread->join();
  return true;
}

bool ZMQControlClient::subscribe(
    const std::string &sock_addr, const std::string &topic,
    std::function<void(const std::string &)> callback) {

  auto &&found = thread_space.subscribers.find(
      sock_addr); // TODO Again change reference methods

  if (found != thread_space.subscribers.end()) {
    LOG_WARN("Subscriber Already Exists");
    return false;
  }
  auto &&val = create_socket(ZMQ_SUB, thread_space.subscribers, sock_addr);
  std::cout << topic.c_str() << " " << strlen(topic.c_str()) << std::endl;
  val.socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  std::cout << sock_addr;
  val.socket->connect(sock_addr); // TODO Change this to pass  as address

  auto &&futureObj = val.exit_signal.get_future();
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::subscription_thread, std::move(futureObj),
      std::move(val.socket), callback);
  return true;
}

bool ZMQControlClient::cancel_subscription(const std::string &reference) {
  std::cout << reference << std::endl;
  thread_space.subscribers[reference]->exit_signal.set_value();
  if (thread_space.subscribers[reference]->thread->joinable())
    thread_space.subscribers[reference]->thread->join();
  std::cout << "made it" << std::endl;
  return true;
}

bool ZMQControlClient::serve(
    const std::string &address,
    std::function<std::string(const std::string &)> callback) {
  auto &&found = thread_space.servers.find(address);
  if (found != thread_space.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }

  auto &&val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto &&futureObj = val.exit_signal.get_future();
  thread_space.servers[address]->socket->bind(address);
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::server_thread_1, std::move(futureObj),
      std::move(thread_space.servers[address]->socket), callback);
  return true;
}

bool ZMQControlClient::serve(const std::string &address,
                             std::function<std::string(void)> callback) {
  auto &&found = thread_space.servers.find(address);
  if (found != thread_space.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }
  auto &&val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto &&futureObj = val.exit_signal.get_future();
  thread_space.servers[address]->socket->bind(address);
  val.thread = std::make_unique<std::thread>(&ZMQControlClient::server_thread_2,
                                             std::move(futureObj),
                                             std::move(val.socket), callback);
  return true;
}

// void ZMQControlClient::periodic(const std::chrono::microseconds &period,
//                                 std::function<void(void)> func) {
//   auto start = std::chrono::steady_clock::now();
//   func();
//   auto wait_time = period + start - std::chrono::steady_clock::now();
//   if (wait_time > std::chrono::milliseconds::zero())
//     std::this_thread::sleep_for(wait_time);
// }

void ZMQControlClient::periodic_publish_thread(
    const std::string sock_addr, std::future<void> exit_signal,
    std::unique_ptr<::zmq::socket_t> socket, std::string topic,
    std::function<std::string &(void)> get_data,
    const std::chrono::microseconds period) {

  std::string temp = sock_addr;
  std::cout << sock_addr << std::endl;
  socket->bind(sock_addr);
  auto start = std::chrono::steady_clock::now();
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    std::cout<<topic<<std::endl;
    start = std::chrono::steady_clock::now();
    s_sendmore(*socket, topic);
    s_send(*socket, get_data());
    auto wait_time = period + start - std::chrono::steady_clock::now();
    if (wait_time > std::chrono::milliseconds::zero())
      std::this_thread::sleep_for(wait_time);
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::periodic_request_thread(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::string server, std::function<std::string(void)> get_data,
    std::function<void(const std::string &)> callback,
    std::chrono::microseconds period) {

  std::cout << "Thread start" << std::endl;

  auto start = std::chrono::steady_clock::now();
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {

    start = std::chrono::steady_clock::now();
    socket->connect(server); // TODO Better way of doing this?
    s_send(*socket, get_data().c_str());
    std::string response = s_recv(*socket);
    callback(response);
    zmq_disconnect(*socket, server.c_str());
    auto wait_time = period + start - std::chrono::steady_clock::now();
    if (wait_time > std::chrono::milliseconds::zero())
      std::this_thread::sleep_for(wait_time);
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::subscription_thread(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::function<void(const std::string &)> callback) {

  std::cout << "Thread start" << std::endl;

  std::string message;
  ::zmq::pollitem_t items[] = {
      {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    zmq::poll(&items[0], 1, 100);
    if (items[0].revents & ZMQ_POLLIN) {
      message = s_recv(*socket);
      message = s_recv(*socket);
      callback(message);
    }
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_1(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::function<std::string(const std::string &)> callback) {

  std::cout << "Thread start" << std::endl;
  ::zmq::pollitem_t items[] = {
      {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    zmq::poll(&items[0], 1, 100);
    if (items[0].revents & ZMQ_POLLIN) {
      std::string request = s_recv(*socket);
      s_send(*socket, callback(request));
    }
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_2(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    const std::function<std::string(void)> callback) {
  std::cout << "Thread start" << std::endl;
  ::zmq::pollitem_t items[] = {
      {static_cast<void *>(*socket.get()), 0, ZMQ_POLLIN, 0}};
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    zmq::poll(&items[0], 1, 100);
    if (items[0].revents & ZMQ_POLLIN) {
      s_recv(*socket); // Throw away
      s_send(*socket, callback());
    }
  }
  std::cout << "Thread end" << std::endl;
}

bool ZMQControlClient::terminate_server(const std::string &reference) {
  thread_space.servers[reference]->exit_signal.set_value();
  thread_space.servers[reference]->thread->join();
  return true;
}
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
