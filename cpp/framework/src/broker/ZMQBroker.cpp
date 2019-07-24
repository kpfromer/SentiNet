
#include "networking/broker/ZMQBroker.hpp"

ZMQBroker::ZMQBroker(const std::string& pub, const std::string& sub)
    : BrokerInterface(pub, sub) {}

ZMQBroker::~ZMQBroker() {}

bool ZMQBroker::initialize() {}

bool ZMQBroker::terminate() {}

bool ZMQBroker::listen() {
  ::zmq::context_t context(context_);

  ::zmq::socket_t frontend(context, ZMQ_SUB);
  frontend.bind(frontend_addr);

  ::zmq::socket_t backend(context, ZMQ_PUB);
  backend.bind(backend_addr);

  ::zmq::pollitem_t items[] = {{frontend, 0, ZMQ_POLLIN, 0},
                               {backend, 0, ZMQ_POLLIN, 0}};

  while (1) {
    ::zmq::message_t message;
    int more;

    ::zmq::poll(&items[0], 2, -1);

    if (items[0].revents & ZMQ_POLLIN) {
      do {
        frontend.recv(&message);
        size_t more_size = sizeof(more);
        frontend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
        backend.send(message, more ? ZMQ_SNDMORE : 0);
      } while (!more);
    }
    if (items[1].revents & ZMQ_POLLIN) {
      do {
        backend.recv(&message);
        size_t more_size = sizeof(more);
        backend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
        frontend.send(message, more ? ZMQ_SNDMORE : 0);
      } while (!more);
    }
  }
  return true;
}
