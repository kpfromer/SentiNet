/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SingleThreadedClientInterface
 * @created     : Thursday Jul 18, 2019 20:09:21 MDT
 */

#ifndef SINGLETHREADEDCLIENTINTERFACE_HPP

#define SINGLETHREADEDCLIENTINTERFACE_HPP

// C++ Includes
#include <memory>

// Project Includes
#include "framework/client/ClientBaseInterface.hpp"

namespace networking {

namespace client {

class SingleThreadedClientInterface : public ClientBaseInterface {
public:
  SingleThreadedClientInterface();
  virtual ~SingleThreadedClientInterface();

private:
};

} // namespace client
} // namespace networking
#endif /* end of include guard SINGLETHREADEDCLIENTINTERFACE_HPP */
