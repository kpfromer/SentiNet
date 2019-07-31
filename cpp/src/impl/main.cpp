/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jul 24, 2019 11:24:28 MDT
 */

#include "networking/zmq/ZMQControlClient.hpp"
#include <unistd.h>
int main() {

  std::unique_ptr<ZMQControlClient> a = std::make_unique<ZMQControlClient>();
  a->publish(
      "tcp://*:5555", "Yo",
      [&]() -> std::string & {
        std::string *value = new std::string("hi");
        return *value;
      },
      std::chrono::microseconds(10));
  sleep(10);
  return 0;
}
