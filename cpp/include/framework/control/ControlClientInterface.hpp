
/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ControlClientInterface
 * @created     : Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef CONTROLINTERFACE_HPP

#define CONTROLINTERFACE_HPP

#include <string>
#include <memory>
#include <functional>

#include "framework/utils/utils.hpp"

class ControlClientInterface {
 public:
  virtual ~ControlClientInterface() = default;

  ////////////////////////// Publish Methods /////////////////////////////
  /**
   * Simple publisher - publishes message to topic
   *
   * @param  topic [Topic name, this can be a topic to a proxy, in which case
   * the message needs an envelope]
   * @param  message     [The data we want to send]
   *
   * @return             [Status of the publish]
   */
  virtual bool publish(const std::string& topic,
                       const std::string& message) = 0;

  // Set a periodic publish / request. i.e. developer attatches a callback to
  // the function to periodically occur
  /**
   * Periodic publisher - publishes message obtained at get_data_to_publish and
   * publishes every period
   *
   * @param  topic [Topic to publish onto. Note that this is not const as it can
   * be changed]
   * @param  get_data_to_publish [Callback called to obtain the message we want
   * to send]
   * @param  period [Polling period to obtain the message from endpoint]
   *
   * @return       [status of the publish]
   */
  virtual bool publish(
      const std::string& sock_addr, std::string& topic,
      const std::function<std::string&(void)>& get_data_to_publish,
      const std::chrono::microseconds& period) = 0;

  virtual bool publish(
      const int& port, std::string& topic,
      const std::function<std::string&(void)>& get_data_to_publish,
      const std::chrono::microseconds& period) {

      return publish(::utils::strings::join(::utils::defaults::SERVER_TCP_PREFIX, port, ":"),
             topic, get_data_to_publish, period);
  }

  virtual bool cancel_periodic_publisher(const std::string&) = 0;
  ////////////////////////// Request Methods /////////////////////////////
  /**
   * Requests a string from an endpoint
   *
   * @param  destination [Destination address of the server]
   * @param  message     [Message to send in request (can be just a blank
   * string)]
   *
   * @return             [The response from the server]
   */
  virtual std::string request(const std::string& destination,
                              const std::string& message) = 0;

  /**
   * Make A request at a regular period
   * @param  destination              [Destination of server]
   * @param  get_data_to_request [Address of the data that we will send] //TODO
   * Probably a better way to do thisw
   * @param  action_to_recieved_data  [What to do to our recieved data]
   * @return                          [Status of request, returns 1 is ends
   * badly]
   */
  virtual bool request(
      std::string& destination,  // Destination can change (not const)
      const std::function<std::string&(void)>&
          get_data_to_request,  // placeholder for string to request with
      const std::function<void(const std::string&)>& action_to_recieved_data,
      const std::chrono::microseconds& period);

  virtual bool cancel_periodic_request(const std::string&) = 0;
  ////////////////////////// Subscribe Methods /////////////////////////////
  /**
   * Subscribe to a topic with a specified callback
   *
   * This is an asynchronous control loop, this method splits off into another
   * threads
   *
   * @param  topic     [Topic to subscribe to]
   * @param  callback    [The callback function to execute on subscription]
   *
   * @return           [Status of the subscription if it ends]
   */
  virtual bool subscribe(
      const std::string& sock_addr, const std::string& topic,
      const std::function<void(const std::string&)>& callback) = 0;

  virtual bool cancel_subscription(const std::string& topic) = 0;

  ////////////////////////// Serve Methods /////////////////////////////
  /**
   * Host a service at the specified address
   *
   * This (like subscribe) is asynchronous and enters into another control loop
   *
   * @param  address [Address to bind to]
   * @param  [name]  [What to do to incomming messages from clients]
   *
   * @return         [Staus if the server dies]
   */
  virtual bool serve(
      const std::string& address,
      const std::function<std::string(const std::string&)>& callback) = 0;

  // Inline when a port is supplied instead of address
  inline bool serve(const int& port,
                    std::function<std::string(const std::string&)>& callback) {
    return serve(
        utils::strings::join(utils::defaults::SERVER_TCP_PREFIX, port, ":"),
        callback);
  }

  /**
   * A Server that doesn't care about incomming arguments
   * @param  address    [Address to serve on]
   * @param  Callback   [The callback to the request]
   * @return            [Status if there is an exit]
   */
  virtual bool serve(const std::string& address,
                     const std::function<std::string(void)>& callback) = 0;

  // Inline when a port is supplied instead of address
  inline bool serve(const int& port,
                    std::function<std::string(void)>& callback) {
    return serve(
        utils::strings::join(utils::defaults::SERVER_TCP_PREFIX, port, ":"),
        callback);
  }

  virtual bool terminate_server(const std::string& address) = 0;
};
#endif /* end of include guard CONTROLINTERFACE_HPP */
