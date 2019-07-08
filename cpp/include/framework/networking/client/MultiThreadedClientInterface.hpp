/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : MultiThreadedClientInterface
 * @created     : Thursday Jul 18, 2019 20:11:03 MDT
 */

#ifndef MULTITHREADEDCLIENTINTERFACE_HPP

#define MULTITHREADEDCLIENTINTERFACE_HPP

// C++ includes
#include <memory>

// Project Includes
#include "framework/networking/client/ClientBaseInterface.hpp"

class MultiThreadedClientInterface {
 public:
  MultiThreadedClientInterface();
  virtual ~MultiThreadedClientInterface();

 private:
  std::list<std::unique_ptr<std::thread>>> request_threads;
  /* private data */
};

#endif /* end of include guard MULTITHREADEDCLIENTINTERFACE_HPP */
