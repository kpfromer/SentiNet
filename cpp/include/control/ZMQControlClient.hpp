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
#include "core/utils/system_config.temp"
#include "core/utils/logging.hpp"
#include "core/control/ControlClientInterface.hpp"
#include "control/zhelpers.hpp"

/**
 * @brief A ZMQControl Client is an implementation of the Control Client
 * Interface. CCI by default is not multithreaded, nor is it as complex as
 * ZMQCC. ZMQ Control Client is structured in a way that allows an application
 * to push subscribers and publishers into a map of said subs and pubs. This
 * allows all messages to be passed in the same process (you don't need two
 * seperate files for a publisher and subscriber) This is useful for modular
 * applications. Eventually, sentinet will allow applications to be uploaded
 * from a central system that allocates resources to the local robot controller.
 * A few important things here. This is multithreaded, ZMQ takes care of race
 * conditions, however, it is strongly advised to read the documentation before
 * diving into the multithreaded aspects of ZMQ. Namely, sockets shouldn't share
 * threads. This is why ZMQ socket thread space exists, to seperate sockets from
 * threads.
 */
class ZMQControlClient : public ControlClientInterface {
public:
  ZMQControlClient(int context_ = 1, const std::string& yaml_system_file = "empty");
  ~ZMQControlClient() {}

public: // TODO Take these out and put htme in controlbase interface
  bool start() override;
  bool quit() override;

public:
  /**
   * @brief In order to publish or request concurrently, you MUST first
   * initialize publisher or client by default, client and publisher don't
   * exist. This would be a method put in the constructor to an implementation
   * specific. The reason is because these two could just cause high overhead.
   * Instead, initializing them explicitly allows the program to only open the
   * necessary sockets
   *
   * @return status
   */
  bool initialize_publisher();

  /**
   * @brief Refer to initialize_publisher
   *
   * @return Status
   */
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
  // TODO I want to be smarter about this, passing params by std::ref, but for
  // now,
  // TODO - Instead of all of these parameters, I need to seperate this into a
  // struct, similar to how a pthread works However, I would like to keep it
  // static, just because of the high overhead of Control Clients
  /**
   * @brief A periodic publisher thread
   *
   * This thread maintains a socket's lifetime within the thread space. It binds
   * and kills the socket automatically inside the thread.
   *
   * @param sock_addr The address to bind to
   * @param exit_signal The exit signal indicating when to leave
   * @param socket The socket to process data on
   * @param topic The topic to publish to, this can be changed
   * @param std::function How the thread gets its string to publish
   * @param period The period to publish using std::chrono
   */
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
  // Honestly I was just too lazy to write out std::map ......
  template <typename T>
  inline socket_thread_space &create_socket(int type, T &map,
                                            const std::string &sock_addr) {
    auto socket_thread = std::make_unique<socket_thread_space>();
    map.emplace(sock_addr, std::move(socket_thread));
    map[sock_addr]->socket = std::make_unique<::zmq::socket_t>(context, type);
    return *map[sock_addr];
  }

/*

  ///////////////////////////////////// Specific Functions for ZMQ CC /////////////////////////////
  public:

    bool publish(const std::string& topic, const std::string& message, const std::chrono::microseconds period) {
      return publish(::utils
    }

    bool publish(const std::string& topic, std::function<std::string&(void)> get_message_to_publish);

    template <typename T>
    T subscribe(const std::string& topic, std::function<T(const std::string& message)> callback);

    bool serve(const std::string& address, const std::string& message);
    
    template <typename T>
    T serve(const std::string& address, std::function<std::string&(const T&)> get_message_to_serve);
*/
};

// Initialize static socket thread space with default constructor

#endif /* end of include guard ZMQCONTROLCLIENT_HPP */
































