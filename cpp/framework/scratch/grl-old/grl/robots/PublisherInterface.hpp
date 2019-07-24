
#ifndef RKS_CONTROL_INTERFACES_PUBLISHERINTERFACE_HPP
#define RKS_CONTROL_INTERFACES_PUBLISHERINTERFACE_HPP

#include "rks/control_interfaces/PublisherBaseInterface"
#include "rks/ConfigurationClient.hpp"

template <typename inBoundMessageT, typename outBoundMessageT>
class PublisherInterface : public PublisherBaseInterface {
 public:
  PublisherInterface(ConfigurationClient& config, unsigned int id,
                     rclcpp::Node::SharedPtr node);
  virtual ~PublisherInterface();

  virtual void timer_callback();
  virtual void update(inBoundMessageT);

 private:
  virtual outBoundMessageT convert_message(inBoundMessageT) = 0;

  rclcpp::Publisher<outBoundMessageT>::SharedPtr publisher;
  rclcpp::Node::SharedPtr node;
  outBoundMessageT message;
}
