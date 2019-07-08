
#ifndef RKS_CONFIGURATIONCLIENT_HPP
#define RKS_CONFIGURATIONCLIENT_HPP

#include <yaml-cpp/yaml.h>

namespace rks {
namespace config {

class ConfigurationClient {
 public:
  ConfigurationClient(const std::string&);

 public:
  YAML::Node get_output(unsigned int, unsigned int);
  YAML::Node get_peripheral(unsigned int);
  YAML::Node get_input(unsigned int, unsigned int);
  YAML::Node get_robot();

  std::string get_dynamic_library_path(std::string);

 private:
  YAML::Node config;
};

}  // namespace config
}  // namespace rks

#endif  // RKS_CONFIGURATIONCLIENT_HPP
