/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQSubscriber
 * @created     : Wednesday Jul 24, 2019 12:33:40 MDT
 */

#include "networking/zmq/ZMQSubscriber.hpp"

ZMQSubscriber::ZMQSubscriber(const std::string &address, int context_) {
  context = zmq::context_t(context_);
  socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);
  socket->connect(address);
}

void ZMQSubscriber::subscribe(const std::string &topic) {
  socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
}

void ZMQSubscriber::listen() {
  while (1) {
    std::string recieve = s_recv(*socket);
    std::cout << recieve << std::endl;
  }
}
