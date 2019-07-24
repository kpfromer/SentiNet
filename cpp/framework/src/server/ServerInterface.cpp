/**
 *  @file ServerInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:11:02 MDT
 *  @bug No Known Bugs
 */

#include "networking/server/ServerInterface.hpp"

using namespace networking::server;

ServerInterface::ServerInterface(const std::string& address_)
    : serving_address(address_) {
  callback = nullptr;
}

ServerInterface::ServerInterface(
    const std::string& add, std::function<std::string(void*, int)> callback_)
    : serving_address(add) {
  set_callback(&callback_);
}

void ServerInterface::set_callback(
    std::function<std::string(void*, int)>* callback_) {
  callback = std::unique_ptr<std::function<std::string(void*, int)>>(callback_);
}
