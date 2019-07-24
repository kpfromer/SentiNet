/**
 *  @file ZMQClient
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
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

// Project includes
#include "networking/client/ClientInterface.hpp"
#include <zmq.hpp>

namespace networking {
namespace client {

class ZMQClient : public ClientInterface {
 public:
  ZMQClient();
  ~ZMQClient();

  virtual bool initialize(int context = 1);
  virtual bool terminate();

 protected:
  virtual bool connect(const std::string& server_address);
  virtual bool disconnect(const std::string& server_address);
  virtual bool make_request(const std::string& request_,
                            std::string& response_);

 private:
  ::zmq::context_t context;
  std::unique_ptr<::zmq::socket_t> socket;
  std::string connected_address;
};

}  // namespace client
}  // namespace networking
#endif /* end of include guard ZMQCLIENT_HPP */
