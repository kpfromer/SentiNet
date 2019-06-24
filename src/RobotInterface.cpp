#include "rks/robot_interfaces/RobotInterface.hpp"

using rks::robot_interfaces::RobotInterface;
using rks::peripheral_interfaces::PeripheralInterface;

RobotInterface::~RobotInterface(){}


PeripheralInterface* RobotInterface::load_peripheral(std::string path_name)
{
  void* handle = dlopen(path_name, RT
}

bool RobotInerface::load_peripherals(ConfigurationClient* config)
{
  void* handel
   return true;
}


//Default Runtime initialization (start listening to control)
bool RobotInterface::initialize(){
  bool initialized = 0;
  //C++ 11
  for(auto const& p : peripherals){
    initialized = initialized && (p.second)->initialize();
  }
  initialized = initialized && this->start_up_scan();
  return initialized;
}

//Runtime termination (stop listening to control)
bool RobotInterface::terminate(){
  bool terminated = 0;
  for(auto const& p : peripherals){
    terminated = terminated && (p.second)->terminate(); //does passing a pointer to a pair reference to a pointer or object?
  }
  terminated = terminated && this->termination_scan();
  return terminated;
}

//Scan all peripherals to make sure they are set up correctly
bool RobotInterface::start_up_scan(){
  return 1;
}

//Scan all peripherals to make sure they are terminated correctly
bool RobotInterface::termination_scan(){
  return 1;
}

//getters
std::string RobotInterface::get_name() const {
  return asset_name;
}

void RobotInterface::get_description(bool verbose = 0){
  printf("====================\n");
  printf("ROBOT NAME : %s\n", asset_name.c_str()); 
  printf("====================\n");
  printf("PERIPHERALS : \n");
  for(auto const& p : peripherals){
    printf("--------------------\n");
    printf("%s: \n", p.first.c_str());
    (p.second)->get_description(verbose);
    printf("--------------------\n");
  } 
  printf("====================\n");
}

//emergency power cut
void RobotInterface::emergency_cut_power(){
  for(auto const& p : peripherals){
    (p.second)->emergency_cut_power();
  }
}
