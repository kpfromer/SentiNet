

/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ControlClientInterface
 * @created     : Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef ZMQCONTROLCLIENT_HPP

#define ZMQCONTROLCLIENT_HPP

#include <future>
#include <thread>

#include "framework/control/ControlClientInterface.hpp"
#include "networking/zmq/zhelpers.hpp"

class ZMQControlClient : public ControlClientInterface {
 public:
  ZMQControlClient(int context_ = 1);
  virtual ~ZMQControlClient();

  /**
   * API Guide - implimentation - inherited methods from ControlClientInterface
   */
 public:
  ///////////////////////////// PUBLISH /////////////////////////////////
  // Publishes on <this thread>
  bool publish(const std::string& topic, const std::string& message) override;

  // Does not execute above fnc - creates a new periodic publisher thread
  bool publish(const std::string& sock_addr, std::string& topic,
               const std::function<std::string&(void)>& get_data_to_publish,
               const std::chrono::microseconds& period) override;

  bool cancel_periodic_publisher(const std::string&);

  ///////////////////////////// REQUEST /////////////////////////////////
  std::string request(const std::string& destination,
                      const std::string& message) override;

  // Does not execute above fnc
  bool request(
      std::string& destination,
      const std::function<std::string&(void)>& get_data_to_request,
      const std::function<void(const std::string&)>& action_to_recieved_data,
      const std::chrono::microseconds& period) override;
  bool cancel_periodic_request(const std::string&);

  ///////////////////////////// SUBSCRIBE /////////////////////////////////
  bool subscribe(
      const std::string& sock_addr, const std::string& topic,
      const std::function<void(const std::string&)>& callback) override;
  bool cancel_subscription(const std::string& topic);

  ///////////////////////////// SERVE /////////////////////////////////
  bool serve(
      const std::string& address,
      const std::function<std::string(const std::string&)>& callback) override;
  bool serve(const std::string& address,
             const std::function<std::string(void)>& callback) override;
  bool terminate_server(const std::string& address);

 private:  // TODO Take these out and put htme in controlbase interface
  void terminate();

  // Thread functions
 private:
  // So that we're not passing an entire object to a thread - these need the
  // necessary arguments (which is a lot)
  void periodic_publish_thread(const std::string& sock_addr,
                                      std::future<void> exit_signal,
                                      std::unique_ptr<::zmq::socket_t> socket,
                                      std::string& topic,
                                      const std::function<std::string&(void)>&,
                                      const std::chrono::microseconds& period);

  void periodic_request_thread(
      std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
      std::string& server, const std::function<std::string&(void)>& get_data,
      const std::function<void(const std::string&)> callback,
      const std::chrono::microseconds& period);

  void subscription_thread(
      std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
      const std::function<void(const std::string&)>& callback);

  void server_thread_1(
      std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
      const std::function<std::string(const std::string&)>& callback);

  void server_thread_2(
      std::future<void> exit_signal, std::unique_ptr<::zmq::socket_t> socket,
      const std::function<std::string&(void)>& callback);

  // Socket code - action items called in publish and request
 private:
   //Assuming socket is already bound to an address
  static void concurrent_publish(std::unique_ptr<::zmq::socket_t> socket,
                                 const std::string& topic,
                                 const std::string& message);

  static std::string concurrent_request(const std::string& server,
                                        std::unique_ptr<::zmq::socket_t> socket,
                                        const std::string& message);

  // Data structures
 private:
  typedef struct {
    std::unique_ptr<::zmq::socket_t> socket;
    std::unique_ptr<std::thread> thread;
    std::promise<void> exit_signal;
  } socket_thread_space;

  typedef struct {
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        subscribers;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        servers;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        periodic_clients;
    std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>
        periodic_publishers;

    std::unique_ptr<::zmq::socket_t> this_publisher;
    std::unique_ptr<::zmq::socket_t> this_client;

    // Keep context the same for now, we could put it in socket_thread_space,
    // but that's for the future No need to really change this either unless
    // high data rates - a future  fix maybe
    ::zmq::context_t context;
  } socket_data;

  typedef struct {
    unsigned int supported_threads;
    std::string control_node_name;
  } control_meta_data;

  control_meta_data meta;
  socket_data data;

 private:
  static inline int get_id(
      std::unordered_map<std::string, std::unique_ptr<socket_thread_space>>&
          map,
      const std::string& reference) {
    return std::hash<std::thread::id>{}(map[reference]->thread->get_id());
  }
  inline void terminate_map_of_threads(
      std::unordered_map<std::string, std::unique_ptr<socket_thread_space>> map,
      std::function<bool(std::string)> terminate_fnc) {
    for (auto const& i : map) terminate_fnc(i.first);
  }

  inline int get_client_id(const std::string& reference) {
    return get_id(data.periodic_clients, reference);
  }
  inline int get_publish_id(const std::string& reference) {
    return get_id(data.periodic_publishers, reference);
  }
  inline int get_subscribe_id(const std::string& reference) {
    return get_id(data.subscribers, reference);
  }
  inline int get_server_id(const std::string& reference) {
    return get_id(data.servers, reference);
  }

  static void periodic(const std::function<void(void)>& func,
                       const std::chrono::microseconds& period);
};

#endif /* end of include guard ZMQCONTROLCLIENT_HPP */
