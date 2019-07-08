/**
 * @author    : theo (theo@$HOSTNAME)
 * @file    : MessageProxyInterface
 * @created   : Saturday Jul 13, 2019 14:35:07 MDT
 */

#ifndef MESSAGEPROXYINTERFACE_HPP

#define MESSAGEPROXYINTERFACE_HPP

class MessageProxyInterface : public Proxy {
 public:
  MessageProxyInterface();
  virtual ~MessageProxyInterface();

 private:
  virtual void notify(const std::string& process_controller,
                      SerialInterface data);

  typedef struct {
    std::unique_ptr<MessagePipeInterface> pipe;
    std::map<std::string, std::unique_ptr<ProcessControllerInterface>>
        process_controllers;
    std::map<std::string, std::unique_ptr<AuditorInterface>> listeners;
  } MessageProxyClients;

  typedef std::function<void(const MessageInterface&)> proxy_callback;

  proxy_callback callback;
  MessageProxyClients clients;
};

#endif /* end of include guard MESSAGEPROXYINTERFACE_HPP */
