/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jul 24, 2019 11:24:28 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"
#include <unistd.h>
int main() {

  std::unique_ptr<ZMQControlClient> a = std::make_unique<ZMQControlClient>();
  a->subscribe("tcp://localhost:5555", "Yo",
               [&](const std::string &value) -> void {
                 std::cout << "Recieved " << value << std::endl;
               });
  sleep(6);
  return 0;
}
