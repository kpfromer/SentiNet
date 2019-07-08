#include "rks/peripheral_interfaces/PeripheralInterface.hpp"

// Constructor non runtime (don't listen to control)
PeripheralInterface::PeripheralInterface(ConfigurationClient& config,
                                         std::string name) {
  this->create_inputs(config, name);
  this->create_outputs(config, name);
}

virtual PeripheralInterface::~PeripheralInterface() {}

virtual void PeripheralInterface::set_immutables(ConfigurationClient& config,
                                                 const unsigned int id) {
  asset_name = config.get_peripheral(id)["name"];
  control = new ControlClientInterface(config.peripheral_control(name));
}

virtual bool PeripheralInterface::initialize() {
  bool initialized = 0;
  for (auto const& m : outputs) {
    initialized = initialized && *(m.second)->initialize();
  }
  for (auto const& m : inputs) {
    initialized = initialized && *(m.second)->initialize();
  }
  initialized =
      initialized && this->start_up_scan() && this->start_up_sequence_test();
  return initialized;
}

virtual bool PeripheralInterface::terminate() {
  bool terminated = 0;
  for (auto const& m : outputs) {
    terminated = terminated && *(m.second)->terminate();
  }
  for (auto const& m : inputs) {
    terminated = terminated && *(m.second)->terminate();
  }
  terminated = terminated && this->termination_scan() &&
               this->termination_sequence_test();
  return terminated;
}

// Scan all peripherals to make sure they are set up correctly
virtual bool PeripheralInterface::start_up_scan() {
  bool satisfied = 0;
  for (auto const& m : outputs) {
    satisfied = satisfied && *(m.second)->start_up_scan();
  }
  for (auto const& m : inputs) {
    satisfied = satisfied && *(m.second)->start_up_scan();
  }
  return satisfied;
}

virtual bool PeripheralInterface::termination_scan() {
  bool satisfied = 0;
  for (auto const& m : outputs) {
    satisfied = satisfied && *(m.second)->termination_scan();
  }
  for (auto const& m : inputs) {
    satisfied = satisfied && *(m.second)->termination_scan();
  }
  return satisfied;
}

virtual bool start_up_sequence_test() { return 1; }

virtual bool termination_sequence_test() { return 1; }

// getters
virtual std::string Peripheral::get_name() const { return asset_name; }

virtual void Robot::print_description(bool verbose = 0) {
  printf("PERIPHERAL NAME : %s\n", asset_name.c_str());
  printf("PERIPHERAL ID : %i\n\n", asset_id.c_str());
  if (verbose) {
    printf("CONTROLS : \n");
    control->print_description();
  }
  printf("<><><><><><><>\n");
  printf("MOTORS: \n");
  for (auto const& m : outputs) {
    printf("%s: \n", m.first.c_str());
    if (verbose) {
      *(p.second)->print_description();
    }
  }
  printf("<><><><><><><>\n");
}

// emergency power cut
virtual void emergency_cut_power() {
  for (auto const& m : outputs) {
    *(m.second)->cut_power();
  }
  for (auto const& m : inputs) {
    *(m.second)->cut_power();
  }
}
