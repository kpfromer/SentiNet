

template <typename messageT>

template <typename outboundMessageT,
          typename inboundMessageT>
PublisherInterface::PublisherInterface(ConfigurationClient& config, unsigned int id, rclcpp::Node::SharedPtr)
{
  name = config["Control"]["Outbound"]["Publishers"][id]["topic"];
  rate = config["Control"]["Outbound"]["Publishers"][id]["rate"];
  node = node_;
  publisher = node->create_publisher<messageT>(name, rate);
  timer_ = node->create_wall_timer(500ms, std::bind(&PublisherInterface::timer_callback, this));
}

template <typename outboundMessageT,
          typename inboundMessageT>
PublisherInterface::~PublisherInterface(){}

template <typename outboundMessageT,
          typename inboundMessageT>
PublisherInterface::timer_callback()
{
  publisher->publish(message);
  count_ ++;
}

template <typename outboundMessageT,
          typename inboundMessageT>
PublisherInterface::update(inboundMessageT val)
{
  message = this->convert_message(val);
}


RosInputInterface::RosInputInterface(ConfigurationClient& config)
: rclcpp::Node(config["Control"]["Outbound"]["NodeName"]
{
  this->initialize_publishers();
}

RosInputInterface::publish_to_all()
{
  for(auto const& p : publishers)
  {
    p->publish();
  }
}
