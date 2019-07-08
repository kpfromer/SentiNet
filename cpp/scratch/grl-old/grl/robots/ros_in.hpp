
#include <rclcpp/rclcpp.hpp>
#include "rks/control_interfaces/ControlClientInterface.hpp"

using rks::control_interfaces::ControlClientInterface;

class PublisherBaseInterface {
 public:
  virtual ~PublisherInterface() = default;

 private:
  virtual void timer_callback() = 0;
  rclcpp::TimerBase::SharedPtr timer_;

  size_t count_;
  const std::string name;
  int rate;
}

template <typename outboundMessageT, typename inboundMessageT>
class PublisherInterface : public PublisherBaseInterface {
 public:
  PublisherInterface(ConfigurationClient& config, unsigned int id,
                     rclcpp::Node::SharedPtr* node);
  virtual ~PublisherInterface();

  virtual void timer_callback();
  virtual void update(inboundMessageT);
  virtual outboundMessageT convert_message(inboundMessageT);

 private:
  rclcpp::Publisher<messageT>::SharedPtr publisher;
  rclcpp::Node::SharedPtr node;
  outboundmessageT message;
}

class RosInputInterface : public rclcpp::Node,
                          public ControlClientInterface {
 public:
  RosInputInterface(ConfigurationClient& config);
  virtual ~RosInputInterface();
  virtual void initialize_publishers() = 0;
  virtual void publish_to_all();

 private:
  std::map<std::string, std::shared_ptr<rclcpp::PublisherBaseInterface>>
      publishers;
}
