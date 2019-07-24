#ifndef RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP
#define RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP

namespace rks {
namespace control_interfaces {

/*
 * Control Client connects to an outside controller, be it over network or via
 * interprocesses communication
 */
class ControlClientInterface {
 public:
  virtual ~ControlClientInterface() = default;

  virtual void

      /* An idea
          template<typename T>
          virtual void connect(T){return connect((void*)&T);}
          */
      void
      connect(void*) = 0;

  virtual void disconnect(void*) = 0;

  virtual void initialize(void*) = 0;
  virtual void terminate(void*) = 0;

  virtual void initialize_listener(void*) = 0;
  virtual void initialize_publisher(void*) = 0;

  virtual void initialize_outbount_client(void*) = 0;
  virtual void initialize_in_bound_server() = 0;

  virtual void make_request(std::string serialized_request) = 0;

  virtual bool send_out_bound(std::string serialized_data) = 0;
  virtual std::string recieve_in_bound(void* descriptor) = 0;

  virtual bool request_content(const std::string& request);
  virtual

      private :

      virtual void
      callback();

  std::unique_ptr<RobotInterface> robot;
};

}  // namespace control_interfaces
}  // namespace rks

#endif  // RKS_CONTROL_INTERFACES_CONTROLCLIENTINTERFACE_HPP
