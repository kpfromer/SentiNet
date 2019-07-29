
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <functional>
#include <list>
#include <future>

namespace utils {
namespace defaults {
const std::string DEFAULT_SOCKET_PREFIX = "default";
const std::string SERVER_TCP_PREFIX = "tcp://*";
const std::string SERVER_UDP_PREFIX = "udp://*";
const std::string LOCAL_HOST_UDP_PREFIX = "udp://127.0.0.1";
const std::string LOCAL_HOST_TCP_PREFIX = "tcp://127.0.0.1";
const std::string DEFAULT_ZMQ_CONTROL_NAME = "ZMQController";
int DEFAULT_PUBLISH_PORT = 5555;
}  // namespace defaults
namespace strings {
template <typename T, typename M>
inline std::string join(const T& first, const M& second) {
  std::stringstream ss;
  ss << first << second;
  return ss.str();
}
template <typename T, typename M, typename D>
inline std::string join(const T& first, const M& second, const D& delim) {
  std::stringstream ss;
  ss << first << delim << second;
  return ss.str();
}
template <typename T>
inline std::string join(std::list<T>& strings) {
  std::stringstream ss;
  for (const auto& i : strings) ss << *i;
  return ss.str();
}
template <typename T, typename M>
inline std::string join(std::list<T>& strings, const M& delim) {
  std::stringstream ss;
  if (strings.empty()) return delim;
  ss << *strings.begin();
  strings.pop_front();
  for (const auto& i : strings) ss << delim << *i;
  return ss.str();
}
}  // namespace strings
namespace threads {
class Stoppable {
  std::promise<void> exitSignal;
  std::future<void> futureObj;

 public:
  Stoppable() : futureObj(exitSignal.get_future()) {}
  Stoppable(Stoppable&& obj)
      : exitSignal(std::move(obj.exitSignal)),
        futureObj(std::move(obj.futureObj)) {}
  Stoppable& operator=(Stoppable&& obj) {
    exitSignal = std::move(obj.exitSignal);
    futureObj = std::move(obj.futureObj);
    return *this;
  }

  // Task need to provide defination  for this function
  // It will be called by thread function
  virtual void run() = 0;

  // Thread function to be executed by thread
  void operator()() { run(); }

  // Checks if thread is requested to stop
  bool stopRequested() {
    // checks if value in future object is available
    if (futureObj.wait_for(std::chrono::milliseconds(0)) ==
        std::future_status::timeout)
      return false;
    return true;
  }
  // Request the thread to stop by setting value in promise object
  void stop() { exitSignal.set_value(); }
};
}  // namespace threads
namespace random {
unsigned char random_char() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return static_cast<unsigned char>(dis(gen));
}

std::string generate_hex(const unsigned int len) {
  std::stringstream ss;
  for (unsigned int i = 0; i < len; i++) {
    auto rc = random_char();
    std::stringstream hexstream;
    hexstream << std::hex << int(rc);
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}

}  // namespace random
}  // namespace utils
