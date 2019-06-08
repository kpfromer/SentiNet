
#ifndef RKS_OUTPUTINTERFACE_HPP
#define RKS_OUTPUTINTERFACE_HPP

#include "rks/ConfigurationClient.hpp"
#include <string>

using rks::config::ConfigurationClient;

namespace rks
{
namespace output
{

class OutputInterface {
  public:
    OutputInterface(ConfigurationClient* config, unsigned int periph, unsigned int out)
    : asset_name(config->get_output(periph, out)["Name"].as<std::string>()){}

    virtual ~OutputInterface() = default;
     
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
    virtual void loop() = 0;
    virtual void emergency_cut_power() = 0;

  private:
    const std::string asset_name;
};

} // namespace output
} // namespace rks

#endif // RKS_OUTPUTINTERFACE_HPP
