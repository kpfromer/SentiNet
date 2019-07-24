/*
 * CS-11 Asn 2, PeripheralInterface.hpp
 * Purpose: An interface for peripheral devices
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP
#define RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP

// RumVision
#include "rks/peripheral_interfaces/PeripheralBaseInterface.hpp"
#include "rks/control_interfaces/ControlClientInterface.hpp"

namespace rks {
namespace peripheral_interfaces {

class PeripheralInterface : PeripheralBaseInterface {
 public:
  PeripheralInterface(ConfigurationClient&, unsigned int);
  virtual ~PeripheralInterface();

 public:
  virtual bool initialize();
  virtual bool terminate();

 protected:
  virtual bool start_up_scan();
  virtual bool termination_scan();

  virtual bool start_up_sequence_test();
  virtual bool termination_sequence_test();

 public:
  virtual std::string get_name() const;
  virtual void get_description(bool);

  // should be overridden - but still has defaults
  virtual void emergency_cut_power();

 protected:
  ControlClientInterface* out_control;

  std::map<std::string, std::shared_ptr<OutputInterface>> outputs;
  std::map<std::string, std::shared_ptr<InputInterface>> inputs;

 private:
  std::string asset_name;
};

}  // namespace peripheral_interfaces
}  // namespace rks

#endif  // RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP
