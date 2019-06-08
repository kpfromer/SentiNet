#include "OutputInterface.hpp"

class DC_Motor : public OutputInterface
{
  public:
    DC_Motor();

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
     void emergency_cut_power();

//  private:
    void motor_write(); //loop
    float convert_throttle();

    float throttle;
    volatile bool failsafe;

    const int resolution;
    const int maxThrottle;
    const int pin;
    const int pwm_throttle_scalar;
    const int pwm_zero_throttle;
};
