
#include "control/ZMQControlClient.hpp"

int main() {

  auto a = std::make_unique<ZMQControlClient>();

  std::string value_to_publish("Hi there");

  a->publish(
      "tcp://localhost:5555", "Topic",
      [&]() -> std::string & { return value_to_publish; },
      std::chrono::milliseconds(1));

  a->subscribe("tcp://localhost:5555", "Topic",
               [&](const std::string &value) -> void {
                 std::cout << "recieved" << value << std::endl;
               });

  a->serve("tcp://localhost:5556", 
      [] (std::string& val) -> std::string { 
          std::cout<<"Got one"; 
          return val; });

  sleep(10);
  a->quit();

  return 0;
}
