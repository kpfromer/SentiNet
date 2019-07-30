
#ifndef ZMQPUBLISHER_HPP
#define ZMQPUBLISHER_HPP

#include "framework/networking/publisher/PublisherInterface.hpp"
#include "networking/zmq/zhelpers.hpp"
#include <sstream>

class ZMQPublisher {
public:
  ZMQPublisher(const std::string &address, int context_);
  ~ZMQPublisher() {}

  void publish(const std::string &topic, const MessageInterface &msg);

private:
  std::unique_ptr<zmq::socket_t> socket;
  zmq::context_t context;
};

#endif
