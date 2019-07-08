/* 
 * CS-11 Asn 2, 
 * Purpose: A Basic Robot class
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP
#define RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP

//C++
#include <map>
#include <memory>


namespace rks
{
namespace robot_interfaces
{

//only pure virtual functions
class RobotBaseInterface
{

  public:
    virtual ~RobotBaseInterface() = default;

  protected:

    //Later - I would like to make this automated as well so that all robot interface does is run the code
    //in action_control tasks
    //For now, loop is the write command that writes to peripheral data objects
    //This is the only function a developer needs to write
    virtual void write() = 0;
};



} // namespace robot_interfaces
} // namespace rks

#endif //RKS_ROBOT_INTERFACES__ROBOTBASEINTERFACE_HPP
