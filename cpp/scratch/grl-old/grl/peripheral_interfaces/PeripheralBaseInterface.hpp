/*
 * CS-11 Asn 2, PeripheralInterface.hpp
 * Purpose: An interface for peripheral devices
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP
#define RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP

#include <map>
#include <memory>

#include "rks/ConfigurationClient.hpp"

namespace rks {
namespace peripheral_interfaces {

// only virtual functions
class PeripheralInterface {
 public:
  virtual ~PeripheralInterface() = default;

 protected:
  virtual void create_inputs(ConfigurationClient&, std::string) = 0;
  virtual void create_outputs(ConfigurationClient&, std::string) = 0;

 public:
  virtual void loop() = 0;
  virtual void update() = 0;
};

}  // namespace peripheral_interfaces
}  // namespace rks

#endif  // RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP
