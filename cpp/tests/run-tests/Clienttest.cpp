/**
 *  @file Clienttest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:45:27 MDT
 *  @bug No Known Bugs
 */

#include <iostream>
#include "networking/client/ZMQClient.hpp"

using namespace networking::client;
int main() {
  ZMQClient* a = new ZMQClient();
  a->initialize();

  std::cout << "here" << std::endl;

  int context = 1;

  std::string value = a->request("tcp://localhost:5555", "hello");

  std::cout << value << std::endl;

  return 0;

}
