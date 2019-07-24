/**
 *  @file ClientInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:24:35 MDT
 *  @bug No Known Bugs
 */

#ifndef CLIENTINTERFACE_HPP

#define CLIENTINTERFACE_HPP

// C++ includes
#include <string>
#include <iostream>

// Project includes

namespace networking {
namespace client {

class ClientInterface {
 public:
  ClientInterface();
  virtual ~ClientInterface() = default;

  // Initialize sub processes
  virtual bool initialize(int) = 0;
  virtual bool terminate() = 0;

  virtual std::string request(const std::string& server_address,
                              const std::string& request_);

 protected:
  virtual bool connect(const std::string& server_address) = 0;
  virtual bool disconnect(const std::string& server_address) = 0;
  virtual bool make_request(const std::string& request_,
                            std::string& response) = 0;

 private:
  virtual bool request_flags(const std::string& server_address,
                             const std::string& request_,
                             std::string& response_);
};

}  // namespace client
}  // namespace networking

#endif /* end of include guard CLIENTINTERFACE_HPP */
