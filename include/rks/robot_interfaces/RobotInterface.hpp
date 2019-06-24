/* 
 * CS-11 Asn 2, 
 * Purpose: A Basic Robot class
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP
#define CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP

//RumVision
#include "rks/robot_interfaces/RobotBaseInterface.hpp"
#include "rks/peripheral_interfaces/PeripheralInterface.hpp"

using rks::config::ConfigurationClient;
using rks::peripheral_interfaces::PeripheralInterface;

namespace rks
{
namespace robot_interfaces
{

//defaults
class RobotInterface : public RobotBaseInterface
{

  public:
    //will probably have to change this later
    RobotInterface(ConfigurationClient* config)
    : asset_name(config->get_robot()["Name"].as<std::string>()){};
    virtual ~RobotInterface();

  public:
    virtual std::string get_name() const;
    virtual void get_description(bool);

  //Kill process
  protected:
    virtual void kill(); 
  private:
    virtual void check_statuses();

  private:
    READ_ONLY_ACTION_OBJECTIVE robot_objective;
    WRITE_ONLY_ACTION_OBJECTIVE* periph_objective;
    
    const std::string asset_name;
};

} // namespace robot_interfaces
} // namespace rks

#endif //CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP
