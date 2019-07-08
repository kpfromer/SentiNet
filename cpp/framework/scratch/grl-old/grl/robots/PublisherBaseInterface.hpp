

#ifndef RKS_CONTROL_INTERFACES_PUBLISHERBASEINTERFACE_HPP
#define RKS_CONTROL_INTERFACES_PUBLISHERBASEINTERFACE_HPP

#include "rclcpp/rclcpp.hpp"
#include "rks/control_interfaces/ControlClientInterface.hpp"
using rks::ControlClientInterface;

namespace rks
{
namespace control_interfaces
{

class PublisherBaseInterface
{
  public:
    virtual ~PublisherInterface() = default;

  private:
    virtual void timer_callback() = 0;
}

} //namespace control_interfaces
} //namespace rks


#endif //RKS_CONTROL_INTERFACES_PUBLISHERBASEINTERFACE_HPP


