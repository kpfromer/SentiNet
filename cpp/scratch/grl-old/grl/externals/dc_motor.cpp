#include "dc_motor.hpp"

inline float DC_Motor::convert_throttle() {
  return (pwm_zero_throttle +
          (constrain(throttle, -maxThrottle, maxThrottle) * scalar));
}

DC_Motor::DC_Motor(const ConfigurationClient& config, const std::string& name,
                   const std::string& name_)
    : OutputInterface(config, name, name_);
{
  pwm_zero_throttle = config.get_output(name, name_)["pwm_zero_throttle"];
  pwm_throttle_scalar = config.get_output(name, name_)["pwm_throttle_scalar"];
  resolution = config.get_output(name, name_)["resolution"];
  pin = config.get_output(name, name_)["pin"];
  maxThrottle = config.get_output(name, name_)["maxThrottle"];
  throttle = 0;
  failsafe = 1;
}

DC_Motor::~DC_Motor() {}

bool DC_Motor::initialze() {
  // technically throttle is already zero, but better be safe
  analogWriteFrequency(pin, pwm_zero_throttle);
  analogWriteResolution(resolution);
  bool proper_init = start_up_scan() && start_up_sequence_test();
  return proper_init;
}

// will add pwm / motor tests
bool start_up_scan() { return 1; }
bool termination_scan() { return 1; }
bool start_up_sequence_test() { return 1; }
bool termination_sequence_test() { return 1; }

void get_description(bool verbose) {
  printf("DC Motor Type - NAME: %s PIN: %d\n", asset.c_str(), pin);
  if (verbose) {
    printf("pwm_zero_throttle: %d\n", pwm_zero_throttle);
    printf("pwm_throttle_scalar: %d\n", pwm_throttle_scalar);
    printf("current throttle: %d\n", throttle);
  }
}

void update(int throttle_) { throttle = throttle_; }

void loop() { motor_write(); }

void emergency_cut_power() {
  analogWriteFrequency(pin, pwm_zero_throttle);

  void DC_Motor::motor_write() {
    if (failsafe) {
      analogWrite(pin, pwm_zero_throttle);
    } else {
      analogWrite(pin, convert_throttle(throttle));
    }
  }
