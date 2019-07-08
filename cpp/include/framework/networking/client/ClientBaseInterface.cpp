/**
 *  @file ClientInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 18:16:39 MDT
 *  @bug No Known Bugs
 */

#include "networking/client/ClientInterface.hpp"

namespace networking {

namespace client {

// pretty boring I know, but there'll probably be more functions in the future
ClientInterface::ClientInterface() {}

std::unique_ptr<MessageBaseInterface> ClientInterface::request(
    const std::string& server_address,
    std::unique_ptr<MessageBaseInterface>& message) {
  auto response_ = std::make_unique<MessageBaseInterface>();
  if (!request_flags(server_address, request_, std::move(response))) {
    std::cout << "ERROR" << std::endl;
    return "";
  }
  return response_;
}

bool ClientInterface::request_flags(
    const std::string& server_address, const std::string& request_,
    std::unique_ptr<MessageBaseInterface> response_) {
  if (!connect(server_address)) {
    std::cout << "Error connecting" << std::endl;
    return false;
  }
  if (!make_request(request_, std::move(response_))) {
    std::cout << "Error request" << std::endl;
    return false;
  }
  if (!disconnect(server_address)) {
    std::cout << "Error disconnecting" << std::endl;
    return false;
  }
  return true;
}

}  // namespace client
}  // namespace networking
