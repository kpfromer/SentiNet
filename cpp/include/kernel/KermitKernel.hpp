/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITKERNEL_HPP

#define KERMITKERNEL_HPP

// Local includes
#include "kernel/SerialPort.hpp"
#include "control/ZMQControlClient.hpp"
#include "messages/common/CmdVel.hpp"
#include "core/utils/strings.hpp"

// C++ includes
#include <memory>
#include <chrono>

namespace Kermit {

constexpr int local_port = 5555;

class KermitKernel : public ZMQControlClient {
public:
  KermitKernel() = delete;
  KermitKernel(const std::string& gun_topic, const std::string& drive_topic);

  virtual ~KermitKernel() = default;

  void set_drive_topic(const std::string& topic);
  void set_gun_topic(const std::string& topic);

  void set_serial(const std::string& port, const int& baud);

  bool loop(const std::chrono::microseconds serial_period);

private:

  bool initialize_control_client(const std::string& address = ::utils::strings::join_d(':', "tcp://localhost*", local_port));

  void recieve_drive_message(const std::string& drive_message);
  void recieve_gun_message(const std::string& gun_message);

  bool send_data(const std::string& val);

  typedef struct KermitOutputs {

    std::unique_ptr<SerialPort> xbee;
    typedef struct {
      int lin;
      int ang;
    } drivetrain_t;
    typedef struct {
      int top_motor;
      int bottom_motor;
    } gun_t;
    drivetrain_t drive;
    gun_t gun;

    KermitOutputs() {
      xbee = nullptr;
      drive = {0, 0};
      gun = {0, 0};
    }

    char buffer[6];

  } KermitOutputs;

  typedef struct KermitMessageProperties {
    KermitMessageProperties() {
      gun_message_handler = nullptr;
      drive_train_message_handler = nullptr;
    }
    std::unique_ptr<Gun> gun_message_handler;
    std::unique_ptr<DriveTrain> drive_train_message_handler;

    std::string buffer;

    std::string drive_topic;
    std::string gun_topic;
  } KermitMessageProperties;

  KermitOutputs kermit;
  KermitMessageProperties message;
};
}
#endif /* end of include guard KERMITKERNEL_HPP */
