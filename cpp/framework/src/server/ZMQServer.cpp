/**
 *  @file ZMQServer
 *  @brief ZMQServer is an interface implimentation of ServerInterface
 *
 *  ZMQServer recieves a request from a client and sends back a reply via ZMQ's
 *  server client pattern. This is useful for things like large data packets (ie
 * camera data or one time requests) that should not be sent over and over via
 * the pub sub pattern
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jul 06, 019 13:30:45 MDT
 *  @bug No Known Bugs
 */

#include "networking/server/ZMQServer.hpp"

using namespace networking::server;

void* networking::server::thread_entry_point(void* args) {
  ZMQServer* ep = (ZMQServer*)args;
  ep->multi_threaded_listener();
  return nullptr;
}

ZMQServer::ZMQServer(const std::string& address) : ServerInterface(address) {
  threads = 1;
}

ZMQServer::ZMQServer(const std::string& address, int threads_)
    : ServerInterface(address) {
  threads = threads_;
}

ZMQServer::ZMQServer(const std::string& address,
                     std::function<std::string(void*, int)> callback)
    : ServerInterface(address, callback) {
  threads = 1;
}

ZMQServer::ZMQServer(const std::string& address, int threads_,
                     std::function<std::string(void*, int)> callback_)
    : ServerInterface(address, callback_) {
  threads = threads_;
}

ZMQServer::~ZMQServer() {}

bool ZMQServer::initialize(int context_) {
  context = ::zmq::context_t(context_);
  socket = std::unique_ptr<::zmq::socket_t>(
      new ::zmq::socket_t(context, threads > 1 ? ZMQ_ROUTER : ZMQ_REP));
  return true;
}

bool ZMQServer::terminate() { return true; }

void ZMQServer::listen() {
  if (threads > 1) {
    // cant put this call globally in this function because once you bind,
    // you need to stay in the same process, it sometimes works
    // but the way c++ compiler works, you never know
    this->socket->bind(serving_address);

    ::zmq::socket_t workers(context, ZMQ_DEALER);
    workers.bind("inproc://workers");

    for (int i = 0; i < threads; i++) {
      pthread_t worker;
      pthread_create(&worker, NULL, thread_entry_point, create_entry_point());
    }

    ::zmq::proxy(*(this->socket), workers, NULL);
  } else {
    single_threaded_listener();
  }
}

void ZMQServer::single_threaded_listener() {
  this->socket->bind(serving_address);
  while (true) {
    ::zmq::message_t request;

    this->socket->recv(request, ::zmq::recv_flags::none);

    std::string result;
    if (callback == nullptr)
      result = ::utils::defaults::callbacks::default_callback(request.data(),
                                                              request.size());
    else
      result = (*callback)(request.data(), request.size());

    ::zmq::message_t reply(result.size());
    memcpy(reply.data(), result.c_str(), result.size());
    socket->send(reply, ::zmq::send_flags::none);
  }
}

void ZMQServer::multi_threaded_listener() {
  // create a socket on the stack so C++ can do the dirty work of deleting
  // threaded sockets
  ::zmq::socket_t worker_socket(this->context, ZMQ_REP);
  worker_socket.connect("inproc://workers");
  while (true) {
    ::zmq::message_t request;

    worker_socket.recv(request, ::zmq::recv_flags::none);

    std::string result;
    if (callback == nullptr)
      result = ::utils::defaults::callbacks::default_callback(request.data(),
                                                              request.size());
    else
      result = (*callback)(request.data(), request.size());

    ::zmq::message_t reply(result.size());
    memcpy(reply.data(), result.c_str(), result.size());
    worker_socket.send(reply, ::zmq::send_flags::none);
  }
}
