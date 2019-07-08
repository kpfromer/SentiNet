/*
 * CS-11 Asn 2, PeripheralInterface.hpp
 * Purpose: An interface for peripheral devices
 *
 * @author Theo Lincke
 * @version 1.0 4/26/19
 */

#ifndef RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP
#define RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP

// C++
#include <string>

// Rumvision
#include "rks/ConfigurationClient.hpp"

using rks::config::ConfigurationClient;

namespace rks {
namespace peripheral_interfaces {

// only virtual functions
class PeripheralBaseInterface {
 public:
  virtual ~PeripheralBaseInterface() = default;

 public:
  virtual void write() = 0;
};

}  // namespace peripheral_interfaces
}  // namespace rks

#endif  // RKS_PERIPHERAL_INTERFACES_PERIPHERALBASEINTERFACE_HPP
