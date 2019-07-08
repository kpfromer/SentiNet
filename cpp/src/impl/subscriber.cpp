/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : subscriber
 * @created     : Wednesday Jul 24, 2019 12:42:09 MDT
 */

#include "networking/zmq/ZMQSubscriber.hpp"

int main() {
  ZMQSubscriber* a = new ZMQSubscriber("tcp://localhost:5555", 1);
  a->subscribe("topic");
  a->listen();
  return 0;
}
