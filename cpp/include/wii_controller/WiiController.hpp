/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : WiiController
 * @created     : Friday Aug 23, 2019 16:30:22 MDT
 */

#ifndef WIICONTROLLER_HPP

#define WIICONTROLLER_HPP

// C++ includes
#include <wiiuse.h>
#include <map>

// Local Includes
#include "networking/zmq/ZMQControlClient.hpp"

class WiiController : public ZMQControlClient {
public:
  WiiController();
  virtual ~WiiController();

  void add_button(const std::string &ref, std::function<void()> callback);
  void add_attribute(const std::string &ref, std::function<void()> callback,
                     std::function<float(struct nunchuk_t *)> get_value);

  void set_zero();
  void execute_callbacks();
  void collect_states();
  void handle_disconnect();
  void wiimote_init();
  short heart_beat();
  void event_loop();

private:
  struct controller_attr {
    virtual void set_zero() = 0;
    std::function<void()> callback;
  };

  struct button : controller_attr {
    long long WIIUSE_ID;
    bool pressed;
  };

  struct attribute : controller_attr {
    std::function<float(struct nunchuk_t *)> get_value;
    float value;
  };

  typedef struct {
    std::vector<attribute> attributes;
    std::vector<button> buttons;
  } ControllerProperties;

  struct wiiuse_attributes {
    wiimote **wiimotes;
    int max_motes;
  };

  wiiuse_attributes wii;
  ControllerProperties controller;
};

#endif /* end of include guard WIICONTROLLER_HPP */
