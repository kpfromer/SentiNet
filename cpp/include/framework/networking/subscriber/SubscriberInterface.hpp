/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SubscriberInterface
 * @created     : Wednesday Jul 24, 2019 12:30:23 MDT
 */

#ifndef SUBSCRIBERINTERFACE_HPP

#define SUBSCRIBERINTERFACE_HPP

#include <string>
#include <memory>

class SubscriberInterface {
public:
  SubscriberInterface() {}
  virtual ~SubscriberInterface() = default;

  virtual void subscribe(const std::string &topic) = 0;
  virtual void listen() = 0;
};

#endif /* end of include guard SUBSCRIBERINTERFACE_HPP */
