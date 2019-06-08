#include "rks/ConfigurationClient.hpp"
#include <iostream>

#include <stdexcept>
#include <fstream>


using namespace rks::config;
inline bool file_exists (const std::string& name) 
{
  std::ifstream f(name.c_str());
  return f.good();
}

ConfigurationClient::ConfigurationClient(const std::string& file_name)
{
  if(!file_exists(file_name)) 
    throw std::invalid_argument( "file does not exist");
  config = YAML::LoadFile(file_name);
}

YAML::Node ConfigurationClient::get_peripheral(unsigned int periph)
{
  if(config["Peripherals"][periph])
    return config["Peripherals"][periph];
  throw std::invalid_argument("Invalid Configuration YAML File");
}

YAML::Node ConfigurationClient::get_output(unsigned int periph, unsigned int output)
{
  if(config["Peripherals"][periph]["Outputs"][output])
    return config["Peripherals"][periph]["Outputs"][output];
  throw std::invalid_argument("Invalid Configuration YAML File");
}

YAML::Node ConfigurationClient::get_input(unsigned int periph, unsigned int input)
{
  if(config["Peripherals"][periph]["Inputs"][input])
    return config["Peripherals"][periph]["Inputs"][input];
  throw std::invalid_argument("Invalid Configuration YAML File");
}

YAML::Node ConfigurationClient::get_robot()
{
  return config;
}
