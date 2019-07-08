/**
 *  @file ClientBaseInterface
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
#include <memory>

// Project includes
#include "framework/messages/MessageBaseInterface.hpp"
#include "framework/networking/client/ClientMacros.hpp"

namespace networking {

namespace client {

class ClientBaseInterface {
 public:
  ClientBaseInterface();

  virtual ~ClientBaseInterface() = default;

  /**
   * @brief At the very least, client will have a name
   */
  void set_client_name(const std::string& name) : client_name(name) {}
  std::string get_client_name() const { return client_name; }

  // Initialize sub processes
  virtual bool initialize(int context = CLIENTDEFAULT) = 0;
  virtual bool terminate() = 0;

  virtual std::unique_ptr<MessageBaseInterface> request(
      const std::string server_address,
      std::unique_ptr<MessageBaseInterface>& message);

 protected:
  virtual bool connect(const std::string& server_address) = 0;
  virtual bool disconnect(const std::string& server_address) = 0;

  virtual bool make_request(std::unique_ptr<MessageBaseInterface> message,
                            std::string& response) = 0;

 private:
  virtual bool request_flags(const std::string& server_address,
                             const std::string& request_,
                             std::unique_ptr<MessageBaseInterface> response_);

  const std::string client_name;
};

}  // namespace client

}  // namespace networking

#endif /* end of include guard CLIENTINTERFACE_HPP */
