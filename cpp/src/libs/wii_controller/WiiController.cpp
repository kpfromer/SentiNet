/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : WiiController
 * @created     : Friday Aug 23, 2019 16:40:21 MDT
 */

#include "wii_controller/WiiController.hpp"

WiiController::WiiController() { initialize_publisher(); }

WiiController::~WiiController() {}

void WiiController::set_zero() {
  for (auto &a : controller.attributes)
    a.second.set_zero();
  for (auto &a : controller.buttons)
    a.second.set_zero();
}

void WiiController::execute_callbacks() {
  for (auto &a : controller.attributes)
    a.second.callback();
  for (auto &a : controller.buttons)
    a.second.callback();
}

bool WiiController::collect_state(const std::string &key,
                                  const int &wiimote_index) {
  auto a = controller.buttons.find(key);
  if (a == controller.buttons.end()) {
    auto a = controller.attributes.find(key);
    if (a == controller.attributes.end())
      return false;
    a->second.value = a->second.get_value();
    return true;
  }
  a->second.value =
      IS_PRESSED(wii.wiimotes[wiimote_index], a->second.WIIUSE_ID);
  return true;
}
