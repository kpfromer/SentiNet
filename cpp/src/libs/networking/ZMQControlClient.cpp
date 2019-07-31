
#include "networking/zmq/ZMQControlClient.hpp"

ZMQControlClient::ZMQControlClient(int context_) 
  : context(context_), this_publisher(nullptr), this_client(nullptr)
{
  
  meta.control_node_name = utils::defaults::DEFAULT_ZMQ_CONTROL_NAME;
  meta.supported_threads = std::thread::hardware_concurrency();
}

ZMQControlClient::~ZMQControlClient() {
  quit();
}

bool ZMQControlClient::start(){
  return true;
}

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

bool ZMQControlClient::publish(const std::string &topic,
                               const std::string &message) {
  if (!this_publisher){
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
  auto&& val = create_socket(ZMQ_PUB, thread_space.periodic_publishers, sock_addr);

  // Create a future promise to exit the thread 
  auto futureObj = val.exit_signal.get_future();

  // Create a new thread - attatched to periodic  publisher
  // Remember to reconstruct the sock_addr because it's constant and 
  // the thread wants mutable
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_publish_thread, sock_addr,
      std::move(futureObj), std::move(val.socket), topic,
      get_data_to_publish, period);
  return true;
}

bool ZMQControlClient::cancel_periodic_publisher(const std::string &reference) {
  thread_space.periodic_publishers[reference]->exit_signal.set_value();
  thread_space.periodic_publishers[reference]->thread->join();
  return true;
}

std::string ZMQControlClient::request(const std::string &destination,
                                      const std::string &message) {
  if (!this_client){
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
  // TODO change destination to a const reference - shouldnt be passing destination
  auto &&found = thread_space.periodic_clients.find(destination);
  if (found != thread_space.periodic_clients.end()) {
    LOG_WARN("Destination already exists");
    return false;
  }

  // create and assign the address of val (a socket_thraed_space) to the map
  auto&& val = create_socket(ZMQ_REQ, thread_space.periodic_clients, destination);

  // Create a future promise to exit
  auto&& futureObj = val.exit_signal.get_future();

  // Our main thread
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::periodic_request_thread, std::move(futureObj),
      std::move(val.socket), destination,
      get_data_to_request, action_to_recieved_data, period);
  return true;
}

bool ZMQControlClient::cancel_periodic_request(const std::string &reference) {
  thread_space.periodic_clients[reference]->exit_signal.set_value();
  thread_space.periodic_clients[reference]->thread->join();
  return true;
}

bool ZMQControlClient::subscribe(
    const std::string &reference, const std::string &topic,
    std::function<void(const std::string &)> callback) {

  auto &&found =
      thread_space.subscribers.find(reference); // TODO Again change reference methods

  if (found != thread_space.subscribers.end()) {
    LOG_WARN("Subscriber Already Exists");
    return false;
  }

  auto&& val = create_socket(ZMQ_SUB, thread_space.subscribers, reference);
  val.socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));

  auto &&futureObj = val.exit_signal.get_future();
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::subscription_thread, std::move(futureObj),
      std::move(val.socket), callback);
  return true;
}

bool ZMQControlClient::cancel_subscription(const std::string &reference) {
  thread_space.subscribers[reference]->exit_signal.set_value();
  thread_space.subscribers[reference]->thread->join();
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

  auto&& val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto &&futureObj = val.exit_signal.get_future();
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
  auto&& val = create_socket(ZMQ_REP, thread_space.servers, address);
  auto &&futureObj = val.exit_signal.get_future();
  val.thread = std::make_unique<std::thread>(
      &ZMQControlClient::server_thread_2, std::move(futureObj),
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

  std::cout << "thread start" << std::endl;
  socket->bind(sock_addr);
  auto start = std::chrono::steady_clock::now();
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout){
    start = std::chrono::steady_clock::now();
    s_sendmore(*socket, topic);
    s_send(*socket, get_data());
    auto wait_time = period + start - std::chrono::steady_clock::now(); 
    if(wait_time > std::chrono::milliseconds::zero())
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
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout){
    start = std::chrono::steady_clock::now();
    socket->connect(server); // TODO Better way of doing this?
    s_send(*socket, get_data().c_str());
    std::string response = s_recv(*socket);
    callback(response);
    zmq_disconnect(*socket, server.c_str());
    auto wait_time = period + start - std::chrono::steady_clock::now(); 
    if(wait_time > std::chrono::milliseconds::zero())
      std::this_thread::sleep_for(wait_time);
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::subscription_thread(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::function<void(const std::string &)> callback) {

  std::cout << "Thread start" << std::endl;
  // socket->connect(topic);
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout){
    callback(s_recv(*socket));
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_1(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::function<std::string (const std::string&)> callback) {

  std::cout << "Thread start" << std::endl;
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
    std::string request = s_recv(*socket);
    s_send(*socket, callback(request));
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_2(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    const std::function<std::string(void)> callback) {
  std::cout << "Thread start" << std::endl;
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    s_recv(*socket);
    s_send(*socket, callback());
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
