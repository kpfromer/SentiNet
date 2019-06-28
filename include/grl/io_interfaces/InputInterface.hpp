
#ifndef RKS_IO_INTERFACES_INPUTINTERFACE_HPP
#define RKS_IO_INTERFACES_INPUTINTERFACE_HPP 

#include "rks/ConfigurationClient.hpp"
#include <string>


using rks::config::ConfigurationClient;

namespace rks
{
namespace input
{

class InputInterface {
  public:
    InputInterface(ConfigurationClient* config, unsigned int periph, unsigned int output)
    : asset_name(config->get_input(periph, output)["Name"].as<std::string>()){}

    virtual ~InputInterface() = default;
    
  public:
    virtual bool initialize() = 0;
    virtual bool terminate() = 0;

  protected:
    virtual bool start_up_scan() = 0;
    virtual bool termination_scan() = 0;

    virtual bool start_up_sequence_test() = 0;
    virtual bool termination_sequence_test() = 0;

  public:
    std::string get_name() const {return asset_name;}
    virtual void get_description(bool);

  public:
    virtual void emergency_cut_power() = 0;

  private:
    const std::string asset_name;
};

} // namespace rks
} // namespace input

#endif //RKS_IO_INTERFACES_INPUTINTERFACE_HPP
