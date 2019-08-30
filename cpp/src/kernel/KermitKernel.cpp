/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Sunday Aug 25, 2019 15:26:38 MDT
 */

#include "kernel/KermitKernel.hpp"

namespace Kermit {

KermitKernel::KermitKernel(const std::string& gun_topic, const std::string& drive_topic)
  : kermit(), message()
{
  set_drive_topic(drive_topic);
  set_gun_topic(gun_topic);
}

void KermitKernel::set_drive_topic(const std::string& topic) {
  message.drive_topic = std::string(topic);
}

void KermitKernel::set_gun_topic(const std::string& topic ) {
  message.gun_topic = std::string(topic);
}

void KermitKernel::set_serial(const std::string& port, const int& baud) {
  kermit.xbee = std::make_unique<SerialPort>(port, baud);
}

bool KermitKernel::loop(const std::chrono::microseconds serial_period) {
  // This is two copies, want to decrease to 1
  // 1. Copy into kermit.buffer
  // 2. Copy into serial.buffer
  kermit.buffer[0] = 'd';
  kermit.buffer[1] = kermit.drive.lin;
  kermit.buffer[2] = kermit.drive.ang;
  kermit.buffer[3] = kermit.gun.top_motor;
  kermit.buffer[4] = kermit.gun.bottom_motor;
  return kermit.xbee->write(kermit.buffer); // string?
}


bool KermitKernel::send_data(const std::string& val) {
  if(!kermit.xbee)
    return false;
  kermit.xbee->write(val);
  return true;
}

bool KermitKernel::

} 
