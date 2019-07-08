/**
 *  @file ProcessControllerInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 09, 2019 18:07:40 MDT
 *  @bug No Known Bugs
 */

#ifndef PROCESSCONTROLLERINTERFACE_HPP

#define PROCESSCONTROLLERINTERFACE_HPP

// C++ includes

// Project includes

namespace grl {
namespace process_control {

/**
 * @brief A process controller recieves messages and converts them to child
 * nodes
 *
 * A process controller deals with splitting the abstract required input to a
 * tangible output value What seperates a process controller from a peripheral
 * is the containment of multiple peripherals Most systems will have only one
 * process controller, but for modular systems, this offers a layer for the
 * control application to distribute tasks in a higher order fashion - abstract
 * tasks for the process controller. Another way of thinking about a process
 * controller is a heuristic item. Peripherals only know what their own inputs
 * and outputs are doing and a process controller sees all peripherals
 * heruistically and maintains guards that create bridges between peripherals
 */
class ProcessControllerInterface {
 public:
  ProcessControllerInterface();
  virtual ~ProcessControllerInterface();

  virtual void initiate() = 0;
  virtual bool terminate() = 0;

  virtual void load_peripheral(const std::string& load_addr);

  /**
   * @brief A List of publishers that publish to certain peripherals
   *
   * Peripherals should be able to switch during runtime, so there should
   * really be no address specific dependency on peripherals, thus there needs
   * to be a middle man between peripherals and process controllers
   */
  std::map<std::string, std::unique_ptr<PublisherInterface>> publishers;

  /**
   * @brief A list of subscribers that listen to the peripherals
   *
   * Although peripherals need to be dynamic, they also need to be constantly
   * updating their parent process controller
   */
  std::map<std::string, std::unique_ptr<SubscriberInterface>> subscribers;

  /**
   * @brief A command subscriber handels incomming messages from the message
   * interface pipe
   */
  std::unique_ptr<SubscriberInterface> command_subscriber;
};

}  // namespace process_control
}  // namespace grl
#endif /* end of include guard PROCESSCONTROLLERINTERFACE_HPP */
