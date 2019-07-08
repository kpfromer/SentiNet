/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQSubscriber
 * @created     : Wednesday Jul 24, 2019 12:31:31 MDT
 */

#ifndef ZMQSUBSCRIBER_HPP

#define ZMQSUBSCRIBER_HPP

#include "networking/zmq/zhelpers.hpp"
#include "framework/networking/subscriber/SubscriberInterface.hpp"

class ZMQSubscriber : public SubscriberInterface {
 public:
  ZMQSubscriber(const std::string& address, int context);
  ~ZMQSubscriber() {}

  void subscribe(const std::string& topic);

  void listen();

 private:
  std::unique_ptr<zmq::socket_t> socket;
  zmq::context_t context;
};

#endif /* end of include guard ZMQSUBSCRIBER_HPP */
