#include "rks/peripheral_interfaces/PeripheralInterface.hpp"

using rks::peripheral_interfaces::PeripheralInterface;

 PeripheralInterface::~PeripheralInterface(){}

bool PeripheralInterface::initialize(){
  bool initialized = 0;
  for(auto const& m : outputs){
    initialized = initialized && (m.second)->initialize();
  }
  for(auto const& m : inputs){
    initialized = initialized && (m.second)->initialize();
  }
  initialized = initialized && this->start_up_scan() && this->start_up_sequence_test();
  return initialized;
}

 bool PeripheralInterface::terminate(){
  bool terminated = 0;
  for(auto const& m : outputs){
    terminated = terminated && (m.second)->terminate();
  }
  for(auto const& m : inputs){
    terminated = terminated && (m.second)->terminate();
  }
  terminated = terminated && this->termination_scan() && this->termination_sequence_test();
  return terminated;
}

//Scan all peripherals to make sure they are set up correctly
bool PeripheralInterface::start_up_scan(){
  return 1;
}

bool PeripheralInterface::termination_scan(){
  return 1;
}

bool PeripheralInterface::start_up_sequence_test(){
  return 1;
}

bool PeripheralInterface::termination_sequence_test(){
  return 1;
}

//getters
 std::string PeripheralInterface::get_name() const {
  return asset_name;
}

void PeripheralInterface::get_description(bool verbose = 0){
  printf("PERIPHERAL NAME : %s\n", asset_name.c_str()); 
  printf("<><><><><><><>\n");
  printf("MOTORS: \n");
  for(auto const& m : outputs){
    printf("%s: \n", m.first.c_str());
    if(verbose){
      (m.second)->get_description(verbose);
    }
  } 
  printf("<><><><><><><>\n");
}

//emergency power cut
void PeripheralInterface::emergency_cut_power(){
  return;
}
