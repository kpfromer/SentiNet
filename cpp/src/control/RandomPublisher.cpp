/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : RandomPublisher
 * @created     : Friday Aug 30, 2019 16:01:03 MDT
 */

#include "control/RandomPublisher.hpp"
#include "messages/common/CmdVel.hpp"

RandomPublisher::RandomPublisher()
{
  initialize_publisher();    
  std::cout<<"Here"<<std::endl;
  DriveTrain temp;
  temp.set_linear(5);
  temp.set_angular(6);
  temp.serialize_to_string(&message);
}

RandomPublisher::~RandomPublisher()
{
}

void RandomPublisher::loop() {
  
  std::cout<<"Here"<<std::endl;
  publish("tcp://*:5555", "drive", [this] (void) -> std::string& { return message; }, std::chrono::seconds(1));
  std::cout<<"Here"<<std::endl;

}
