
/**
 * @file ControlClientInterface
 * @brief A Control Client is the uer endpoint for network patterns
 *
 * A Control Client's sole job is to publish, subscribe, serve, and request.
 * What that means to an individual implimentation is up to the developer.
 * This is a pure virtual class, except for utility functions (implimentations
 * that make the main four methods easier). Implimenting a ControlClient
 * involves routing messages over some sort of network communication.
 *
 * CC's I would like to develop
 *
 * ZMQ
 * ROS / ROS2
 * RabbitMQ
 *
 * TODO
 * Need some sort of bridge service to connect over device interfaces
 *
 * @author  theo (theo@$Theo-Lenovo-Yoga-Arch)
 * @created  Saturday Jul 27, 2019 09:54:19 MDT
 */

#ifndef CONTROLINTERFACE_HPP

#define CONTROLINTERFACE_HPP

// C++ includes
#include <chrono>
#include <functional>
#include <string>

// Local Includes

/**
 * @brief Control Client interface is the communication client in the Control
 * Base application
 *
 * This is a pure interface class. The object oriented approach is going to
 * allow flexibility with the type of communication clients that are
 * implimented. However, we should not get too carried away with an object
 * oriented approach. I thought about adding helper functions to this class, but
 * each Control client will have its own resources that it is utilizing. Thus,
 * I'm keeping only the base functions so that CCs have something to derrive
 * from. If anything, this is a template for how to publish onto a certain
 * control client.
 */
class ControlClientInterface {
public:
  virtual ~ControlClientInterface() = default;

  /**
   * @brief A pure virtual way to start and stop a CC. This should be up to the
   * developer
   *
   * Start and stop are both dependent on the application, they are purely
   * virtual to encourage the developer to impliment a method for terminating
   * their CC.
   *
   * @return Status of start
   */
  virtual bool start(int params = 0) = 0;

  /**
   * @brief Quit should return satisfactorily and clean up all thread or
   * publishing objects
   *
   * @return Status of quit
   */
  virtual bool quit(int params = 0) = 0;

  /**
   * Core API
   * main methods are
   *
   * publish
   * subscribe
   * serve
   * request
   */

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
  virtual bool publish(const std::string &topic,
                       const std::string &message) = 0;

  // Set a periodic publish / request. i.e. developer attatches a callback to
  // the function to periodically occur
  /**
   * Periodic publisher - publishes message obtained at get_data_to_publish and
   * publishes every period
   *
   * @note it's ok to copy
   *
   * @param  topic [Topic to publish onto. Note that this is not const as it can
   * be changed]
   * @param  get_data_to_publish [Callback called to obtain the message we want
   * to send]
   * @param  period [Polling period to obtain the message from endpoint]
   *
   * @return       [status of the publish]
   */
  virtual bool publish(std::string broker_frontend, std::string topic,
                       std::function<std::string &(void)> get_data_to_publish,
                       const std::chrono::microseconds &period) = 0;

  typedef struct publish_params {
    publish_params() = delete;
    publish_params(const std::string &broker, const std::string &topic_) {
      broker_frontend = broker;
      topic = topic_;
    }
    publish_params(const std::string &broker, const std::string &topic_,
                   std::function<std::string &(void)> get_data_,
                   const std::chrono::microseconds period_) {
      broker_frontend = broker;
      topic = topic_;
      get_data = get_data_;
      period = period_;
    }
    std::string broker_frontend;
    std::string topic;
    std::function<std::string &(void)> get_data;
    std::chrono::microseconds period;
  } publish_params;

  bool request(publish_params &params) {
    return publish(params.broker_frontend, params.topic, params.get_data,
                   params.period);
  }

  virtual bool cancel_periodic_publisher(const std::string &) = 0;

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
  virtual std::string request(const std::string &destination,
                              const std::string &message) = 0;

  /**
   * Make A request at a regular period
   * @param  destination              [Destination of server]
   * @param  get_data_to_request [Address of the data that we will send] //TODO
   * Probably a better way to do thisw
   * @param  action_to_recieved_data  [What to do to our recieved data]
   * @return                          [Status of request, returns 1 is ends
   * badly]
   */
  virtual bool
  request(std::string &destination, // Destination can change (not const)
          std::function<std::string &(const std::string &)> get_data_to_request,
          std::function<void(const std::string &)> action_to_recieved_data,
          const std::chrono::microseconds &period) = 0;

  typedef struct {
    std::string destination;
    std::function<std::string &(const std::string &)> get_data_to_request;
    std::function<void(const std::string &)> callback;
    const std::chrono::microseconds period;
  } periodic_request_parameters;

  bool request(periodic_request_parameters &params) {
    return request(params.destination, params.get_data_to_request,
                   params.callback, params.period);
  }

  virtual bool cancel_periodic_request(const std::string &) = 0;

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
  virtual bool subscribe(const std::string &sock_addr, const std::string &topic,
                         std::function<void(const std::string &)> callback) = 0;

  virtual bool cancel_subscription(const std::string &topic) = 0;

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
  virtual bool
  serve(const std::string &address,
        std::function<std::string(const std::string &)> callback) = 0;

  /**
   * A Server that doesn't care about incomming arguments
   * @param  address    [Address to serve on]
   * @param  Callback   [The callback to the request]
   * @return            [Status if there is an exit]
   */
  virtual bool serve(const std::string &address,
                     std::function<std::string(void)> callback) = 0;

  virtual bool terminate_server(const std::string &address) = 0;
};
#endif /* end of include guard CONTROLINTERFACE_HPP */
