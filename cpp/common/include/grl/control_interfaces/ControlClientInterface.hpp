
#ifndef RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP
#define RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP


namespace rks
{
namespace control_interfaces
{

class ControlClientInterface{
  public:
    virtual ~ControlClientInterface() = default;
  
    virtual auto send_out_bound() = 0;
    virtual auto recieve_in_bound() = 0;

};

} // namespace control_interfaces
} // namespace rks

#endif //RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP
