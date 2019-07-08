#include <iostream>
#include "ConfigurationClient.hpp"

int main() {
  rks::config::ConfigurationClient test(
      "/home/theo/dev/main/ros2_ws/rumvision/cpp/rks/include/rks/tests/"
      "example_conf.yaml");
  std::cout << test.get_output(0, 0) << std::endl << std::endl;
  std::cout << test.get_input(0, 0) << std::endl << std::endl;
  std::cout << test.get_peripheral(0) << std::endl << std::endl;
  return 0;
}
