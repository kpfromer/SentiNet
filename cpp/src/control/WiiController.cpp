/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : WiiController
 * @created     : Friday Aug 23, 2019 16:40:21 MDT
 */

#include "control/WiiController.hpp"

WiiController::WiiController() {
  initialize_publisher();
  controller = std::make_unique<controller_s>();
  controller->stick->home.callback = nullptr;
  controller->stick->one.callback = nullptr;
  controller->stick->two.callback = nullptr;
  controller->stick->up.callback = nullptr;
  controller->stick->down.callback = nullptr;
  controller->stick->right.callback = nullptr;
  controller->stick->left.callback = nullptr;
  controller->stick->a.callback = nullptr;
  controller->stick->b.callback = nullptr;
  controller->stick->minus.callback = nullptr;
  controller->stick->plus.callback = nullptr;

  robot = std::make_unique<robot_s>();
  peripheral *p = new peripheral;
  robot->p = p;
  robot->p->start = start_robot;
  robot->p->stop = stop_robot;
  robot->p->loop = loop_robot;

  robot->gun = nullptr;
  robot->camera = nullptr;
  robot->drive = nullptr;
  robot->period = .01;
}

WiiController::~WiiController() {
  if (robot->p)
    delete robot->p;
  if (robot->camera)
    delete robot->camera;
  if (robot->gun)
    delete robot->gun;
  if (robot->drive)
    delete robot->drive;
}

void WiiController::register_controller_callback(
    button_s &button, void *(*callback)(controller_s *, robot_s *)) {
  button.callback = callback;
}

void WiiController::add_gun() {
  robot->gun = new gun;
  *robot->gun = create_gun();
}

void WiiController::add_drive_train(int a, int b, int c) {
  robot->drive = new drive_train;
  *robot->drive = create_drive_train(a, b, c);
}

void WiiController::add_camera() {
  robot->camera = new camera;
  *robot->camera = create_camera();
}

void WiiController::set_opt(int val) { robot_setopt(robot.get(), val); }

void WiiController::set_period(float per) { robot->period = per; }

void WiiController::set_debug() { set_opt(DEBUG); }

void WiiController::set_verbose() { set_opt(VERBOSE); }

void WiiController::set_var_speed() { set_opt(VAR_SPEED); }

void WiiController::set_disc_lin_ang() { set_opt(DISCLINANG); }

void WiiController::set_non_lin() { set_opt(NONLIN); }

std::string &WiiController::get_dt() { msg. }
