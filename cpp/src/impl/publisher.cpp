/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : publisher
 * @created     : Wednesday Jul 24, 2019 12:42:50 MDT
 */

#include "networking/zmq/ZMQPublisher.hpp"
#include "messages/debug/DebugMessage.h"

int main() {
  ZMQPublisher* a = new ZMQPublisher("tcp://*:5555", 1);

  DebugMessage* msg = new DebugMessage();
  msg->set_name("Theo");
  msg->set_id(5);
  msg->set_extra("extra");
  while (1) {
    a->publish("topic", *msg);
    sleep(2);
  }
  return 0;
}
