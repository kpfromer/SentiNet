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
    
    RobotInterface(ConfigurationClient* config)
    : asset_name(config->get_robot()["Name"].as<std::string>()){};

    virtual ~RobotInterface();

  //TODO
  public:
    virtual bool load_peripherals(ConfigurationClient* config);

  public:
    virtual bool initialize();
    virtual bool terminate();

  protected:
    //peripheral probing
    virtual bool start_up_scan();
    virtual bool termination_scan();

    virtual bool start_up_status();
    virtual bool termination_status();

  public:

    virtual std::string get_name() const;
    virtual void get_description(bool);

  protected:

    virtual void emergency_cut_power(); 

  protected:
    std::map<std::string, std::shared_ptr<PeripheralInterface>> peripherals;


  private:
    void* action_objective;

    const std::string asset_name;
};

} // namespace robot_interfaces
} // namespace rks

#endif //CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP
