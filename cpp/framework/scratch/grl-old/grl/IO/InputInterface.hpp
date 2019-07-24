
#ifndef RKS_INPUTINTERFACE_HPP
#define RKS_INPUTINTERFACE_HPP

#include "rks/ConfigurationClient.hpp"

namespace rks {
namespace input {

class InputInterface {
 public:
  InputInterface(ConfigurationClient&, unsigned int periph,
                 unsigned int output) {
    asset_name = config.get_input(periph, output)["name"];
  }

  virtual ~InputInterface() = 0;

 public:
  virtual bool initialize() = 0;
  virtual bool terminate() = 0;

 protected:
  virtual bool start_up_scan() = 0;
  virtual bool termination_scan() = 0;

  virtual bool start_up_sequence_test() = 0;
  virtual bool termination_sequence_test() = 0;

 public:
  std::string get_name() const { return asset_name; }
  void get_description(bool) = 0;

 public:
  virtual void update() = 0;
  virtual void emergency_cut_power() = 0;

 private:
  std::string asset_name;
};

}  // namespace input
}  // namespace rks

#endif  // RKS_INPUTINTERFACE_HPP
