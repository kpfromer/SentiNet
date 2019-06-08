/* 
 * CS-11 Asn 2, 
 * Purpose: A Basic Robot class
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP
#define RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP

//RumVision
#include "rks/ConfigurationClient.hpp"

//C++
#include <map>
#include <memory>

using rks::config::ConfigurationClient;

namespace rks
{
namespace robot_interfaces
{

//only virtual functions
class RobotBaseInterface
{

  public:
    virtual ~RobotBaseInterface() = default;

  protected:
    virtual void create_peripherals(ConfigurationClient& config) = 0;

  protected:
    virtual void update() = 0;
};



} // namespace robot_interfaces
} // namespace rks

#endif //RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP

