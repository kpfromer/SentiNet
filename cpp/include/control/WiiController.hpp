/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : WiiController
 * @created     : Friday Aug 23, 2019 16:30:22 MDT
 */

#ifndef WIICONTROLLER_HPP

#define WIICONTROLLER_HPP

// C++ includes
#include <memory>

// Third party includes
#include "wii_controller.h"
#include "robot_control.h"

// Local Includes
#include "control/ZMQControlClient.hpp"
#include "messages/common/CmdVel.hpp"

class WiiController : public ZMQControlClient {
public:
  WiiController();
  virtual ~WiiController();

  static void register_controller_callback(button_s &,
                                           void *(*cb)(controller_s *,
                                                       robot_s *));

  virtual bool start() override;

public:
  std::string &get_dt();
  std::string &get_gun();

  void set_opt(int);
  void set_period(float);

  void set_debug();
  void set_verbose();
  void set_var_speed();
  void set_disc_lin_ang();
  void set_non_lin();

  void add_gun();
  void add_camera();
  void add_drive_train(int, int, int);

  void loop();

private:
  std::unique_ptr<robot_s> robot;
  std::unique_ptr<controller_s> controller;

  typedef struct {
    std::string dt;
    std::string gun;
    DriveTrain dt_msg;
    Gun gun_msg;
  } message;

  message msg;
};

#endif /* end of include guard WIICONTROLLER_HPP */
