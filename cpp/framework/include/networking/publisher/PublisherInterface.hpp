

namespace networking
{
namespace publisher
{

class PublisherInterface
{
  public:
    
    PublisherInterface(const std::string address);
    virtual ~Publisher() = 0;

    virtual bool initialize() = 0;
    virtual bool terminate() = 0;

    virtual bool write(const std::string& data, const std::string& topic = "") = 0;

  protected:
    std::string address;
};
}
}
