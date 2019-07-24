

#include "networking/broker/BrokerInterface.hpp"

namespace networking {
namespace broker {

class ZMQBroker : public BrokerInterface {
 public:
  ZMQBroker(const std::string&, const std::string&);
  ZMQBroker(int in_port, int out_port);

  ~ZMQBroker();

  virtual bool initialize();
  virtual bool terminate();

  virtual bool listen();

 private:
};
}  // namespace broker
}  // namespace networking
