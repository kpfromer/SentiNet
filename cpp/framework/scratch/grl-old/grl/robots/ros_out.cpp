#include <rclcpp/rclcpp.hpp>

template <class T>
class SubscriberInterface : rclcpp::Node{
  public:
    SubscriberInterface(std::string, std::string) : Node(std::string);
    ~SubscriberInterface(){}

  public:
    void timer_callback();
    virtual void update_message() = 0;

  private:
    rclcpp::Subscriber<T>::SharedPtr subscriber;
    std::string topic_name;
    T message;
}

//Might not be ppossible
class RosOutputInterface {
  public:
    RosOutputInterface(std::string);
    ~RosOutputInterface(){}
    virtual void initialize_all_subscribers() = 0;

    void 
}
