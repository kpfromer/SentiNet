#ifndef UTILS_SYSTEM_DEFAULTS_HPP
#define UTILS_SYSTEM_DEFAULTS_HPP

namespace utils {
namespace defaults {
const std::string DEFAULT_SOCKET_PREFIX = "default";
const std::string SERVER_TCP_PREFIX = "tcp://*";
const std::string SERVER_UDP_PREFIX = "udp://*";
const std::string LOCAL_HOST_UDP_PREFIX = "udp://127.0.0.1";
const std::string LOCAL_HOST_TCP_PREFIX = "tcp://127.0.0.1";
const std::string DEFAULT_ZMQ_CONTROL_NAME = "ZMQController";
} // namespace defaults
namespace ports {
std::queue<int> avaliable_ports;
// A list of ports with a description - I don't know if this is needed
std::map<int, std::string> in_use_ports;

static inline void init_ports() {
  YAML::Node ports = YAML::LoadFile("system.yml")["ports"];
  for (std::size_t i = 0; i < ports.size(); i++) {
    // extract ranges
    std::vector<std::string> discretes =
        ::utils::strings::parse(ports[i].as<std::string>(), ',');
    for (const auto &i : discretes) {
      std::vector<std::string> values = ::utils::strings::parse(i, ':');
      int high = atoi(values.back().c_str());
      int low = atoi(values.front().c_str());
      for (int j = low; j <= high; j++)
        avaliable_ports.push(j);
    }
  }
}

static inline int get_port(const std::string &port_description = "In Use") {
  if (avaliable_ports.size() <= 0)
    return -1;
  int i = avaliable_ports.front();
  in_use_ports.emplace(i, port_description);
  avaliable_ports.pop();
  return i;
}
} // namespace ports
} // namespace utils

#endif
