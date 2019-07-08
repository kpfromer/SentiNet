#include <iostream>
#include "dc_motor.hpp"

int main() {
  DC_Motor my_motor;
  std::cout << my_motor.initialize() << std::endl;
  std::cout << my_motor.throttle << std::endl;
  my_motor.update(10);
  std::cout << my_motor.convert_throttle() << std::endl;

  std::cout << my_motor.convert_throttle() << std::endl;
  std::cout << std::endl;
  my_motor.get_description(true);
}
