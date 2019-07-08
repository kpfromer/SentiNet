/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : PublisherInterface
 * @created     : Saturday Jul 20, 2019 11:34:36 MDT
 */

#ifndef PUBLISHERINTERFACE_HPP

#define PUBLISHERINTERFACE_HPP

// Project Includes
#include "framework/networking/messages/MessageInterface.hpp"

// C++ includes
#include <string>
#include <memory>

class PublisherInterface {
 public:
  PublisherInterface() {}
  virtual ~PublisherInterface() = default;

  virtual void publish(const std::string topic,
                       const MessageInterface& msg) = 0;
};

#endif /* end of include guard PUBLISHERINTERFACE_HPP */
