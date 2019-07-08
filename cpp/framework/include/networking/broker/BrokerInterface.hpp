

namespace networking
{
namespace broker
{

/**
* @brief A Broker is designed to recieve messages from a pubsub / client server and push the 
* message forward, its essentially a router
*/
class BrokerInterface
{
  public:
    BrokerInterface(const std::string, const std::string);
    virtual ~BrokerInterface() = default;

    virtual bool initialize() = 0;
    virtual bool terminate() = 0;

    virtual bool listen() = 0;

  private:
    const std::string frontend_addr;
    const std::string backend_addr;

};

}
}
