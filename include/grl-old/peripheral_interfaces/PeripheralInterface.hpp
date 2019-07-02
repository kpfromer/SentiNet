#ifndef RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP
#define RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP

//RumVision
#include "rks/peripheral_interfaces/PeripheralBaseInterface.hpp"
#include "rks/ConfigurationClient.hpp"
#include "rks/OutputInterface.hpp"
#include "rks/InputInterface.hpp"

//C++
#include <map>
#include <memory>

using rks::config::ConfigurationClient;
using rks::output::OutputInterface;
using rks::input::InputInterface;

namespace rks
{
namespace peripheral_interfaces
{


class PeripheralInterface : public PeripheralBaseInterface
{
  public:
    PeripheralInterface(ConfigurationClient* config, unsigned int id)
    : asset_name(std::string);
    //: asset_name(config->get_peripheral(id)["Name"].as<std::string>()){}

    virtual ~PeripheralInterface();

  //TODO
  public:
    virtual bool load_inputs(ConfigurationClient* config unsigned int);
    virtual bool load_outputs(ConfigurationClient* config, unsigned int);

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

    //should be overridden - but still has defaults
    virtual void emergency_cut_power();

  protected:

    std::map<std::string, std::shared_ptr<OutputInterface>> outputs;
    std::map<std::string, std::shared_ptr<InputInterface>> inputs;

  private:
    void* io_objective;
    const std::string asset_name;
}; 

} //namespace peripheral_interfaces
} //namespace rks

#endif //RKS_PERIPHERAL_INTERFACES_PERIPHERALINTERFACE_HPP
