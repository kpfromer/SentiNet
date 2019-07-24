/*
 * CS-11 Asn 2,
 * Purpose: A Basic Robot class
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP
#define CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP

// RumVision
#include "rks/robot_interfaces/RobotBaseInterface.hpp"
#include "rks/PeripheralInterface.hpp"
#include "rks/ConfigurationClient.hpp"
#include "rks/control_interfaces/ControlClientInterface.hpp"

namespace rks {
namespace robot_interfaces {

/* Robot Interface
 *
 * JOB - To update all peripherals
 *
 * LINKER - action_objective - this is an arbitrary pointer to a pool of data
 * that is mutated by ControlClientInterface and is read only for Robot
 *
 */
// defaults
class RobotInterface : public RobotInterfaceInterface {
 public:
  // DEFAULT CONSTRUCTOR
  RobotInterfaceInterface(ConfigurationClient& config);
  // DEFAULT DESTRUCTOR
  virtual ~RobotInterfaceInterface();

  //@TODO
  // Dynamically load all peripherals
 public:
  void load_peripherals(YAML::Node peripheral_libraries);

  // PROCESS TIME INTIALIZATION
  /* These initialization protocols assume all peripherals are loaded and
   * Configured with their constants and methods
   * These are runtime initializations
   */
 public:
  virtual bool initialize();
  virtual bool terminate();

  // STARTUP TESTS - OPTIONAL - DEFAULT TRUE

  // Probe the peripherals and test their status
  virtual bool probe_peripherals_startup();
  virtual bool probe_peripherals_termination();

  // Run any tests - ie move around the room a bit or calibrate readings
  // MACRO TESTS
  virtual bool start_up_status();
  virtual bool termination_status();

 protected:
  // Cut all power
  virtual void emergency_cut_power();

  // Accessors
 public:
  virtual std::string get_name() const;
  virtual void get_description(bool);

 protected:
  std::map<std::string, std::thread>> peripheral_threads;
  std::map<std::string, std::shared_ptr<PeripheralInterface>> peripherals;

 private:
  // Instead of initializing constants at runtime, constants are a pointer so a
  // user can point their robot system to a set of commands - ie linear angular
  // movement controls, this way a user can alter the data during runtime
  void* action_objective;

  const std::string asset_name;
};

}  // namespace robot_interfaces
}  // namespace rks

#endif  // CRKCPP_ROBOT_S__ROBOTINTERFACE_HPP
