#include "dc_motor.hpp"
#include "PeripheralInterface.hpp"

class FourWheelDrive : PeripheralInterface {
 public:
  FourWheelDrive(ConfigurationClient&, std::string)
      : PeripheralInterface(ConfigurationClient&, std::string);

  ~FourWheelDrive();

  void create_inputs(ConfigurationClient&, std::string);
  void create_outputs(ConfigurationClient&, std::string);

 public:
  void loop();
  void update(int, int);
  void emergency_cut_power();

 private:
  int linear;
  int angular;
}
