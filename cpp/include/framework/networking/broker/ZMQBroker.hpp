/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ZMQBroker
 * @created     : Saturday Jul 13, 2019 15:18:47 MDT
 */

#ifndef ZMQBROKER_HPP

#define ZMQBROKER_HPP

class ZMQBroker {
 public:
  ZMQBroker();
  virtual ~ZMQBroker();

  virtual void initialize()

      private : ::zmq::context_t context;
  std::unique_ptr<::zmq::socket_t> frontend_sock;
  std::unique_ptr<::zmq::socket_t> backend_sock;
  /* private data */
};

#endif /* end of include guard ZMQBROKER_HPP */
