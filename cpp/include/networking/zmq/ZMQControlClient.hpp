

/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ControlClientInterface
 * @created     : Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef ZMQCONTROLCLIENT_HPP

#define ZMQCONTROLCLIENT_HPP

// C++ includes
#include <future>
#include <thread>

// Local includes
#include "framework/control/ControlClientInterface.hpp"
#include "networking/zmq/zhelpers.hpp"

class ZMQControlClient : public ControlClientInterface {
public:
  ZMQControlClient(int context_ = 1);
  ~ZMQControlClient() {}

public: // TODO Take these out and put htme in controlbase interface
  bool start() override;
  bool quit() override;
  /*
    bool initialize_outbound(bool requester, bool publisher)
    {
      if(requester)
        thread_spacethis_client =
        std::make_unique<::zmq::socket_t>(context, ZMQ_REQ);
      if(publisher){
        data.this_publisher =
        std::make_unique<::zmq::socket_t>(data.context, ZMQ_PUB);
        data.this_publisher->bind(::utils::strings::join_d(
          ':', ::utils::defaults::SERVER_TCP_PREFIX,
          ::utils::ports::get_port()));
        }
        return true; //TODO
    }
  */

public:
  bool initialize_publisher();
  bool initialize_client();

  /**
   * API Guide - implimentation - inherited methods from ControlClientInterface
   */
public:
  ///////////////////////////// PUBLISH /////////////////////////////////
  // Publishes on <this thread>
  bool publish(const std::string &topic, const std::string &message) override;

  // Does not execute above fnc - creates a new periodic publisher thread
  bool publish(std::string sock_addr, std::string topic,
               std::function<std::string &(void)> get_data_to_publish,
               std::chrono::microseconds period) override;

  bool cancel_periodic_publisher(const std::string &) override;

  ///////////////////////////// REQUEST /////////////////////////////////
  std::string request(const std::string &destination,
                      const std::string &message) override;

  // Does not execute above fnc
  bool request(
      std::string &destination,
      std::function<std::string &(void)> get_data_to_request,
      const std::function<void(const std::string &)> action_to_recieved_data,
      const std::chrono::microseconds &period) override;

  bool cancel_periodic_request(const std::string &) override;

  ///////////////////////////// SUBSCRIBE /////////////////////////////////
  bool subscribe(const std::string &sock_addr, const std::string &topic,
                 std::function<void(const std::string &)> callback) override;

  bool cancel_subscription(const std::string &topic) override;

  ///////////////////////////// SERVE /////////////////////////////////
  bool serve(const std::string &address,
             std::function<std::string(const std::string &)> callback) override;

  bool serve(const std::string &address,
             std::function<std::string(void)> callback) override;
  bool terminate_server(const std::string &address) override;

  // Thread functions
private:
  /**
   * @brief The function call to a new periodic publisher thread
   *
   * @param sock_addr The sock address to bind to
   * @note sock_addr can be initialized earlier with bind, but it's better to
   * create all socket->bind() calls in the same thread space, that way we're
   * not sharing threads between sockets
   *
   * @param exit_signal checked every loop - turned on to quit
   * @param socket The socket to move
   * @note socket is not shared between threads, this is just the refernece, it
   * is bare bones constructed in the call to begin the thread
   *
   * @param topic The topic the socket publishes to
   * @note topic is mutable - allows the topic to be changed mid execution
   *
   * @param get_publish A method that returns the string to publish
   * @note this can just be a simple getter method, but be careful with race
   * conditions
   *
   * @param period The period of the publish //TODO change to frequency
   */
  // For all const & references, you must refere to it as std::ref(value)
  // http://www.davidespataro.it/modern-c-concurrency-how-to-use-a-thread-object-correctly-and-common-pitfalls/
  // THIS IS REALLY IMPORTANT
  // std::ref https://en.cppreference.com/w/cpp/utility/functional/ref WILL
  // CAUSE RACE CONDITIONS THIS NEEDS TO BE HANDLED WITH EXTREME CARE!!!!!!!!!
  // static void periodic_publish_thread(const std::string sock_addr,        //
  // Even if the thread wanted to, it can't change this
  //                              std::future<void> exit_signal,             //
  //                              Can be changed from outside
  //                              std::unique_ptr<::zmq::socket_t> socket,   //
  //                              A unique pointer, so once this thread is
  //                              called,
  //                                                                         //
  //                                                                         The
  //                                                                         thread
  //                                                                         has
  //                                                                         complete
  //                                                                         control
  //                                                                         over
  //                                                                         it
  //                              std::string &topic,                        //
  //                              Important! This can be changed, and it's an
  //                              address std::function<std::string &(void)>&,
  //                              // This can also be changed const
  //                              std::chrono::microseconds period);   // Cannot
  //                              be changed

  // I want to be smarter about this, passing params by std::ref, but for now,
  // I'm going to allow all threads to copy
  static void periodic_publish_thread(const std::string sock_addr,
                                      std::future<void> exit_signal,
                                      std::unique_ptr<::zmq::socket_t> socket,
                                      std::string topic,
                                      std::function<std::string &(void)>,
                                      const std::chrono::microseconds period);

  /**
   * @brief Thread That requests data from a server asynchronously at a (most
   * likely large) period
   * @note This should be used lightly, requests are more purposful and
   * shouldn't be automated too much
   *
   * @param exit_signal The exit signal to tell the thread to quit
   * @param socket The socket We pass get our data from
   * @param server The server address
   * @note server is mutable so it is possible to change the client address
   *
   * @param get_data The function to get the request to request with
   * @param callback What to do with the recieved data from the server //TODO
   * build an abstract system that logs strings and requests that is seperate
   * from logger
   * @param period The period to publish
   */
  static void periodic_request_thread(
      std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
      std::string server, std::function<std::string(void)> get_data,
      std::function<void(const std::string &)> callback,
      std::chrono::microseconds period);

  /**
   * @brief Subscribes as a thread to a topic
   *
   * @param exit_signal The signal set to false that exits the thread
   * @param socket The socket to get data on
   * @param callback What to do to the incomming data
   */
  static void
  subscription_thread(std::future<void> exit_signal,
                      std::unique_ptr<::zmq::socket_t> socket,
                      std::function<void(const std::string &)> callback);

  /**
   * @brief A server thread that cares about the request made
   *
   * @param exit_signal The signal that cuts the thread off
   * @param socket The socket to recieve and send data from
   * @param callback The callback function that recieves a string
   */
  static void
  server_thread_1(std::future<void> exit_signal,
                  std::unique_ptr<::zmq::socket_t> socket,
                  std::function<std::string(const std::string &)> callback);

  /**
   * @brief A server thread that doesnt care about the request made
   *
   * @param exit_signal The signal that cuts the thread off
   * @param socket The socket to recieve and send data from
   * @param callback The callback function that just returns a string
   */
  static void server_thread_2(std::future<void> exit_signal,
                              std::unique_ptr<::zmq::socket_t> socket,
                              std::function<std::string(void)> callback);

  // Socket code - action items called in publish and request
private:
  // Assuming socket is already bound to an address
  static void concurrent_publish(std::unique_ptr<::zmq::socket_t> socket,
                                 const std::string &topic,
                                 const std::string &message);

  std::string concurrent_request(const std::string &server,
                                 std::unique_ptr<::zmq::socket_t> socket,
                                 const std::string &message);

  // Data structures
private:
  /**
   * @brief A Place to store all socket information
   * @note sockets are not to be shared between threads but contexts should be
   */
  typedef struct {
    std::unique_ptr<::zmq::socket_t> socket;
    std::unique_ptr<std::thread> thread;
    std::promise<void> exit_signal;
  } socket_thread_space;

  /**
   * @brief The data structure of our sockets.
   */
  typedef struct socket_data_s {
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        subscribers;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        servers;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        periodic_clients;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        periodic_publishers;
  } socket_data;

  /**
   * @brief Individual data per entity
   */
  typedef struct {
    unsigned int supported_threads;
    std::string control_node_name;
  } control_meta_data;

private:
  // Meta information - unique
  control_meta_data meta;

  // Thread spaces - Shared
  socket_data thread_space;

  // Unique per control client (ideally the same between processes)
  ::zmq::context_t context;

  // Unique per control client
  std::unique_ptr<::zmq::socket_t> this_publisher;
  std::unique_ptr<::zmq::socket_t> this_client;

  // Helper Functions
private:
  // inline int get_id(const std::string& descriptor, const std::string&
  // reference);
  //
  // inline int get_client_id(const std::string &reference) {
  //   return get_id("clients", reference);
  // }
  // inline int get_publish_id(const std::string &reference) {
  //   return get_id("publishers", reference);
  // }
  // inline int get_subscribe_id(const std::string &reference) {
  //   return get_id("subscribers", reference);
  // }
  // inline int get_server_id(const std::string &reference) {
  //   return get_id("servers", reference);
  // }
  //
  // Not utilizing this function yet
  // But I would like to http://stroustrup.com/wrapper.pdf
  // static void periodic(const std::chrono::microseconds &period,
  //                      std::function<void(void)> func);

  // Honestly I was just too lazy to write out std::map ......
  template <typename T>
  inline socket_thread_space &create_socket(int type, T &map,
                                            const std::string &sock_addr) {
    auto socket_thread = std::make_unique<socket_thread_space>();
    map.emplace(sock_addr, std::move(socket_thread));
    map[sock_addr]->socket = std::make_unique<::zmq::socket_t>(context, type);
    return *map[sock_addr];
  }
};

// Initialize static socket thread space with default constructor

#endif /* end of include guard ZMQCONTROLCLIENT_HPP */
