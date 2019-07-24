#include "dc_motor.hpp"
#include <string>
#include <iostream>

template <class T>
inline float constrain(const T& x, const T& a, const T& b) {
  if (x < a)
    return a;
  else if (b < x)
    return b;
  else
    return x;
}

float DC_Motor::convert_throttle() {
  return (pwm_zero_throttle +
          (constrain((const float&)throttle, (const float&)-maxThrottle,
                     (const float&)maxThrottle) *
           pwm_throttle_scalar));
}

DC_Motor::DC_Motor()
    : OutputInterface(),
      maxThrottle(10),
      pwm_zero_throttle(10),
      pin(1),
      pwm_throttle_scalar(5),
      resolution(11) {
  throttle = 0;
  failsafe = 1;
}

DC_Motor::~DC_Motor() {}

bool DC_Motor::initialize() {
  std::cout << "writing frequency on " << pin << " freq " << pwm_zero_throttle
            << std::endl;
  std::cout << "writing resolution " << resolution << std::endl;
  bool proper_init = start_up_scan() && start_up_sequence_test();
  return proper_init;
}

bool DC_Motor::terminate() {
  std::cout << "terminate boi" << std::endl;
  bool proper_term = termination_scan() && termination_sequence_test();
  return proper_term;
}

// will add pwm / motor tests
bool DC_Motor::start_up_scan() { return 1; }
bool DC_Motor::termination_scan() { return 1; }
bool DC_Motor::start_up_sequence_test() { return 1; }
bool DC_Motor::termination_sequence_test() { return 1; }

void DC_Motor::get_description(bool verbose) {
  std::cout << throttle << std::endl;
  printf("DC Motor Type - NAME: %s PIN: %d\n", asset_name.c_str(), pin);
  if (verbose) {
    printf("pwm_zero_throttle: %d\n", pwm_zero_throttle);
    printf("pwm_throttle_scalar: %d\n", pwm_throttle_scalar);
    printf("current throttle: %f\n", throttle);
  }
}

void DC_Motor::update(int throttle_) { throttle = throttle_; }

void DC_Motor::loop() { motor_write(); }

void DC_Motor::emergency_cut_power() { std::cout << "emergency" << std::endl; }

void DC_Motor::motor_write() {
  if (failsafe) {
    std::cout << "writig to" << pin << "throttle" << pwm_zero_throttle
              << std::endl;
  } else {
    std::cout << "writing to" << pin << "thottle " << convert_throttle()
              << std::endl;
  }
}
