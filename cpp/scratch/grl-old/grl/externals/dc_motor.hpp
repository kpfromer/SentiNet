#include "Output_impl.hpp"

class DC_Motor : OutputInterface {
 public:
  DC_Motor(const ConfigurationClient& config, const std::string&,
           const std::string&)
      : OutputInterface(ConfigurationClient & config, const std::string&,
                        const std::string&);

  ~DC_Motor();

  bool initialize();
  bool terminate();

  bool start_up_scan();
  bool termination_scan();

  bool start_up_sequence_test();
  bool termination_sequence_test();

  void get_description(bool);
  void loop();
  void update(int throttle);
  void emergency_cur_power();

 private:
  void motor_write();  // loop
  inline float convert_throttle();

  float throttle;
  volatile bool failsafe;

  const int maxThrottle;
  const int pin;
  const int pwm_throttle_scalar;
  const int pwm_zero_throttle;
};
