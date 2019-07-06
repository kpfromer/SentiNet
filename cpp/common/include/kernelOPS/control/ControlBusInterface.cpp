



#include <cstdint>
#include <memory>
#include <list>

class ControlBusInterface
{
  public:

    ControlBusInterface();
    virtual ~ControlBusInterface() = default;

    virtual connect(const std::string& pub_addr, const std::string& sub_addr) = 0;

    virtual void disconnect() = 0;

    virtual bool subscribe(const std::string& topic) = 0;

    virtual bool subscribe(const std::list<std::string>& topics)
    {
      for(auto const t& : topics)
      {
        subscribe(t);
      }
    }

    virtual bool write(const std::string& data, const std::string& topic) = 0;

    virtual bool read(std::string& data) = 0;
 }
