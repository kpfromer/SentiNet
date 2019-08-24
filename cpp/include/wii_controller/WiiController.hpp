/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : WiiController
 * @created     : Friday Aug 23, 2019 16:30:22 MDT
 */

#ifndef WIICONTROLLER_HPP

#define WIICONTROLLER_HPP

// C++ includes
#include <map>

// Local Includes

// Local Includes
#include "networking/zmq/ZMQControlClient.hpp"

class WiiController : public ZMQControlClient {
public:
  WiiController();
  virtual ~WiiController();

  
private:
  std::unique_ptr<
};

#endif /* end of include guard WIICONTROLLER_HPP */
