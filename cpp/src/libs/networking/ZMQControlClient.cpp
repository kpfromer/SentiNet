
#include "networking/zmq/ZMQControlClient.hpp"

ZMQControlClient::ZMQControlClient(int context_) {
  data.context = ::zmq::context_t(context_);
  data.this_publisher = nullptr;
  data.this_client = nullptr;

  meta.control_node_name = utils::defaults::DEFAULT_ZMQ_CONTROL_NAME;
  meta.supported_threads = std::thread::hardware_concurrency();
}

ZMQControlClient::~ZMQControlClient() {}

void ZMQControlClient::terminate() {
  if (data.this_publisher)
    data.this_publisher = nullptr;
  if (data.this_client)
    data.this_client = nullptr;
  for (auto const &i : data.subscribers)
    cancel_subscription(i.first);
  for (auto const &i : data.servers)
    terminate_server(i.first);
  for (auto const &i : data.periodic_publishers)
    cancel_periodic_publisher(i.first);
  for (auto const &i : data.periodic_clients)
    cancel_periodic_request(i.first);
  /** //TODO make this work so that we can terminate with them specialized
  functions and so it looks pretty terminate_map_of_threads(data.subscribers,
  [=](const std::string&) -> bool {return cancel_subscription();});
  terminate_map_of_threads(data.servers, [=](const std::string&) -> bool {return
  terminate_server();}); terminate_map_of_threads(data.periodic_publishers,
  [=](const std::string&) -> bool {return cancel_periodic_request();});
  terminate_map_of_threads(data.periodic_clients, [=](const std::string&) ->
  bool {return cancel_periodic_request();});
  */
}

bool ZMQControlClient::publish(const std::string &topic,
                               const std::string &message) {
  if (!data.this_publisher) {
    data.this_publisher =
        std::make_unique<::zmq::socket_t>(data.context, ZMQ_PUB);
    data.this_publisher->bind(::utils::strings::join(
        ::utils::defaults::SERVER_TCP_PREFIX,
        ::utils::defaults::DEFAULT_PUBLISH_PORT++,
        ":")); // Absolutely horrible, change how ports are allocated
  }
  concurrent_publish(std::move(data.this_publisher), topic, message);
  return true; // TODO change this
}

bool ZMQControlClient::publish(
    const std::string &sock_addr, std::string &topic,
    const std::function<std::string &(void)> &get_data_to_publish,
    const std::chrono::microseconds &period) {
  auto &&found = data.periodic_publishers.find(topic);
  if (found != data.periodic_publishers.end()) {
    std::cout << "Topic publisher already exists" << std::endl;
    return false;
  }
  auto socket_thread = std::make_unique<socket_thread_space>();
  data.periodic_publishers.emplace(topic, std::move(socket_thread));
  data.periodic_publishers[sock_addr]->socket =
      std::make_unique<::zmq::socket_t>(data.context, ZMQ_PUB);
  auto futureObj =
      data.periodic_publishers[sock_addr]->exit_signal.get_future();
  data.periodic_publishers[sock_addr]->thread = std::make_unique<std::thread>(
      this, &ZMQControlClient::periodic_publish_thread, sock_addr,
      std::move(futureObj),
      std::move(data.periodic_publishers[sock_addr]->socket), topic,
      get_data_to_publish, period);
  return true;
}

bool ZMQControlClient::cancel_periodic_publisher(const std::string &reference) {
  data.periodic_publishers[reference]->exit_signal.set_value();
  data.periodic_publishers[reference]->thread->join();
  return true;
}

std::string ZMQControlClient::request(const std::string &destination,
                                      const std::string &message) {
  if (!data.this_client)
    data.this_client = std::make_unique<::zmq::socket_t>(data.context, ZMQ_REQ);
  return concurrent_request(destination, std::move(data.this_client), message);
}

bool ZMQControlClient::request(
    std::string &destination,
    const std::function<std::string &(void)> &get_data_to_request,
    const std::function<void(const std::string &)> &action_to_recieved_data,
    const std::chrono::microseconds &period) {
  auto &&found = data.periodic_clients.find(
      destination); // TODO CHANGE the identifier method, we cant be passing
                    // string destination
  if (found != data.periodic_clients.end()) {
    std::cout << "Destination client already exists" << std::endl;
    return false;
  }
  auto socket_thread = std::make_unique<socket_thread_space>();
  data.periodic_clients.emplace(destination, std::move(socket_thread));
  data.periodic_clients[destination]->socket =
      std::make_unique<::zmq::socket_t>(data.context, ZMQ_REQ);
  auto futureObj = data.periodic_clients[destination]->exit_signal.get_future();
  data.periodic_clients[destination]->thread = std::make_unique<std::thread>(
      periodic_request_thread, std::move(futureObj),
      std::move(data.periodic_clients[destination]->socket), destination,
      get_data_to_request, action_to_recieved_data, period);
  return true;
}

bool ZMQControlClient::cancel_periodic_request(const std::string &reference) {
  data.periodic_clients[reference]->exit_signal.set_value();
  data.periodic_clients[reference]->thread->join();
  return true;
}

bool ZMQControlClient::subscribe(
    const std::string &reference, const std::string &topic,
    const std::function<void(const std::string &)> &callback) {
  auto &&found =
      data.subscribers.find(reference); // TODO Again change reference methods
  if (found != data.subscribers.end()) {
    std::cout << "Subscriber already exists" << std::endl;
    return false;
  }
  auto socket_thread = std::make_unique<socket_thread_space>();
  data.subscribers.emplace(reference, std::move(socket_thread));
  data.subscribers[reference]->socket =
      std::make_unique<::zmq::socket_t>(data.context, ZMQ_SUB);
  data.subscribers[reference]->socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(),
                                                  strlen(topic.c_str()));
  auto &&futureObj = data.periodic_clients[reference]->exit_signal.get_future();
  data.subscribers[reference]->thread = std::make_unique<std::thread>(
      subscription_thread, std::move(futureObj),
      std::move(data.subscribers[reference]->socket), callback);
  return true;
}

bool ZMQControlClient::cancel_subscription(const std::string &reference) {
  data.subscribers[reference]->exit_signal.set_value();
  data.subscribers[reference]->thread->join();
  return true;
}

bool ZMQControlClient::serve(
    const std::string &address,
    const std::function<std::string(const std::string &)> &callback) {
  auto &&found = data.servers.find(address);
  if (found != data.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }
  auto socket_thread = std::make_unique<socket_thread_space>();
  data.servers.emplace(address, std::move(socket_thread));
  data.servers[address]->socket =
      std::make_unique<::zmq::socket_t>(data.context, ZMQ_REP);
  auto &&futureObj = data.servers[address]->exit_signal.get_future();
  data.servers[address]->thread = std::make_unique<std::thread>(
      server_thread_1, std::move(futureObj),
      std::move(data.servers[address]->socket), callback);
  return true;
}

bool ZMQControlClient::serve(const std::string &address,
                             const std::function<std::string(void)> &callback) {
  auto &&found = data.servers.find(address);
  if (found != data.servers.end()) {
    std::cout << "Already have a server there bud" << std::endl;
    return false;
  }
  auto socket_thread = std::make_unique<socket_thread_space>();
  data.servers.emplace(address, std::move(socket_thread));
  data.servers[address]->socket =
      std::make_unique<::zmq::socket_t>(data.context, ZMQ_REP);
  auto &&futureObj = data.servers[address]->exit_signal.get_future();
  data.servers[address]->thread = std::make_unique<std::thread>(
      server_thread_2, std::move(futureObj),
      std::move(data.servers[address]->socket), callback);
  return true;
}

void ZMQControlClient::periodic(const std::function<void(void)> &func,
                                const std::chrono::microseconds &period) {
  auto start = std::chrono::steady_clock::now();
  func();
  auto wait_time = period + start - std::chrono::steady_clock::now();
  if (wait_time > std::chrono::milliseconds::zero())
    std::this_thread::sleep_for(wait_time);
}

void ZMQControlClient::periodic_publish_thread(
    const std::string &sock_addr, std::future<void> exit_signal,
    std::unique_ptr<::zmq::socket_t> socket, std::string &topic,
    const std::function<std::string &(void)> &get_data,
    const std::chrono::microseconds &period) {
  std::cout << "thread start" << std::endl;
  socket->bind(sock_addr);
  auto execute = [socket = std::move(socket), &topic, &get_data](void) -> void {
    s_sendmore(*socket, topic);
    s_send(*socket, get_data());
  };
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout)
    periodic(execute, period);
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::periodic_request_thread(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    std::string &server, const std::function<std::string &(void)> &get_data,
    const std::function<void(const std::string &)> callback,
    const std::chrono::microseconds &period) {
  std::cout << "Thread start" << std::endl;
  auto execute = [socket = std::move(socket), &server, &get_data,
                  &callback](void) -> void {
    socket->connect(server); // TODO Better way of doing this?
    s_send(*socket, get_data().c_str());
    std::string response = s_recv(*socket);
    callback(response);
    zmq_disconnect(*socket, server.c_str());
  };
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout)
    periodic(execute, period);
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::subscription_thread(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    const std::function<void(const std::string &)> &callback) {
  std::cout << "Thread start" << std::endl;
  // socket->connect(topic);

  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout)
    callback(s_recv(*socket));
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_1(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    const std::function<std::string &(void)> &callback) {
  std::cout << "Thread start" << std::endl;
  socket->bind(address);
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    std::string request = s_recv(*socket);
    s_send(*socket, callback(request));
  }
  std::cout << "Thread end" << std::endl;
}

void ZMQControlClient::server_thread_2(
    std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
    const std::function<std::string &(void)> &callback) {
  std::cout << "Thread start" << std::endl;
  while (exit_signal.wait_for(std::chrono::milliseconds(0)) ==
         std::future_status::timeout) {
    s_recv(*socket);
    s_send(*socket, callback());
  }
  std::cout << "Thread end" << std::endl;
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
