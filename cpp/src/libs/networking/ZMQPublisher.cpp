
#include "networking/zmq/ZMQPublisher.hpp"

ZMQPublisher::ZMQPublisher(const std::string& address, int context_) {
  context = zmq::context_t(context_);
  socket = std::make_unique<::zmq::socket_t>(context, ZMQ_PUB);
  socket->bind(address);
}

void ZMQPublisher::publish(const std::string& topic,
                           const MessageInterface& msg) {
  std::stringstream message;
  message << msg;
  s_sendmore(*socket, topic);
  s_send(*socket, message.str());
}
