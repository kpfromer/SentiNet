/**
 *  @file ZMQClient
 *  @brief Client implimentation
 *
 *  This client implimentation in ZMQ creates a sink
 *  generator pattern, multithreaded or multi process
 *  i.e. it sets up worker threads that pump tasks
 *  onto  the bus
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:44:13 MDT
 *  @bug No Known Bugs
 */

#ifndef ZMQCLIENT_HPP

#define ZMQCLIENT_HPP

// C++ includes
#include <memory>
#include <string>
#include <iostream>
#include <cstdlib>
#include <pthread.h>

// Project includes
#include "networking/client/ClientInterface.hpp"
#include <zmqhelpers.hpp>

namespace networking {
namespace client {

class ZMQClient : public ClientInterface {
public:
  ZMQClient();
  ~ZMQClient();

  virtual bool initialize(int context = 1);
  virtual bool terminate();

protected:
  virtual bool connect(const std::string &server_address);
  virtual bool disconnect(const std::string &server_address);
  virtual bool make_request(const std::string &request_,
                            std::string &response_);

  virtual bool serialize(const std::ostream *input) = 0;

  void *(*entry_fnc)(void *);

private:
  ::zmq::context_t context;
  std::unique_ptr<::zmq::socket_t> socket;
  std::string connected_address;
};

} // namespace client
} // namespace networking
#endif /* end of include guard ZMQCLIENT_HPP */
