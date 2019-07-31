

#include "framework/control/ControlClientInterface.hpp"
#include "networking/zmq/zhelpers.hpp"

class ZMQControlClient : public ControlClientInterface {
public:
  ZMQControlClient(int context_ = 1);
  virtual ~ZMQControlClient();

  // Lifecycle
public:
  bool start();
  bool quit();
  bool initialize_outbound(bool = true, bool publisher = true);

public:
};
