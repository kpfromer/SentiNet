



#include "networking/bridge/NetworkBridgeAdapter.hpp"

ZMQNetworkBridgeAdapter::ZMQNetworkBridgeAdapter(const std::string& sub_addr, const std::string& pub_addr, int context_num)
  : frontend_addr(sub_addr), backend_addr(pub_addr)
{
  context_ = context_num;
}

ZMQNetworkBridgeAdapter::~ZMQNetworkBridgeAdapter()
{}

void ZMQNetworkBridgeAdapter::listen()
{
  ::zmq::context_t context(context_);
  
  ::zmq::socket_t frontend(context, ZMQ_SUB);
  frontend.connect(frontend_addr);

  ::zmq::socket_t backend(context, ZMQ_PUB);
  backend.bind(backend_addr);

  frontend.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  while(1)
  {
    while(1) {
      ::zmq::message_t message;
      int more;
      size_t more;
      size_t more_size = sizeof(more);

      frontend.recv(&message);
      frontend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
      backend.send(message, more ? ZMQ_SNDMORE : 0);
      if(!more)
        break;
    }
  }
}
