/**
 *  @file ServerInterface
 *  @brief An interface  file for Server.
 *
 *  A Server is something that is able to listen on repeate
 *  for a requesting client. There is multithreading support in some
 *  servers, but default threads is 1
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:41:03 MDT
 *  @bug No Known Bugs
 */

#ifndef SERVERINTERFACE_HPP

#define SERVERINTERFACE_HPP

// C++ includes
#include <string>
#include <functional>
#include <cstring>
#include <memory>
#include <iostream>

// Project includes
#include "defaults.hpp"
#include "common/RecieverInterface.hpp"

namespace networking {
namespace server {

/**
 * @brief Server intterface serves a template for implimented servers
 */
class ServerInterface : public RecieverInterface {
 public:
  /**
   * @brief Default constructor, sets address and threads to 1
   *
   * This constructor simply sets serving address to address and threads
   * to 1 it is here to be able to interface with other implimented server
   * types
   *
   * @param address self's address
   */
  ServerInterface(const std::string& address);

  /**
   * @brief Secondary constructor when there multithreading is possible
   *
   * Be careful setting multiple threads, it's really easy and has a very
   * strong effect on the opperating system. This constructor puts an
   * amount of worker threads that all listen to the primary thread. Note
   * that worker threads are initialized on the stack in the method
   * multi_threaded_listener.
   *
   * @param address serving_address
   * @param threads number of threads
   */
  ServerInterface(const std::string& address,
                  std::function<std::string(void*, int)> callback);

  void set_callback(std::function<std::string(void*, int)>* callback_);

  /**
   * @brief Default Destructor
   */
  virtual ~ServerInterface() = default;

  /**
   * @brief Up to the programmer to impliment initialize via the api
   *
   * Origionally this method passed a void pointer as a context, but
   * an int should capture as much information as we need for now. If
   * needed, there can be meta information stored in the bits of the int
   *
   * @param context A number that may or may not need to be passed through
   * the method.
   *
   * @return Status, was this executed okay, if not, send an error message
   */

  virtual bool initialize(int context) = 0;

  /**
   * @brief Up to the programmer to impliment terminate via the api
   *
   * Terminate shuts down all processes, be it disconnect from ports
   * or stop threaded processes
   *
   * @return Status, was this executed okay, if not, send an error message
   */
  virtual bool terminate() = 0;

  /**
   * @brief Main control loop
   *
   * This is very much up to the implimentation. This can be infinitely
   * looped or not. ZMQ for example has ports tied to processes. I tried
   * splitting this method using fork(), but I didn't think it was worth
   * it so I prefer to keep this as an infinite loop that listens to the
   * outside world
   *
   * TODO impliment a handler to handle kill signals so we can quit listen
   */
  virtual void listen() = 0;

 protected:
  /**
   * @brief Address to serve on
   *
   * This address can be anything from a ros topic or a zmq uri
   */
  const std::string serving_address;

  std::unique_ptr<std::function<std::string(void*, int)>> callback;
};

}  // namespace server
}  // namespace networking

#endif /* end of include guard SERVERINTERFACE_HPP */
