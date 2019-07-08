/**
 *  @file ActionProcess
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jun 29, 2019 14:19:31 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTIONPROCESS_HPP

#define ACTIONPROCESS_HPP

// C++ includes
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>

// Project includes
#include "TriggerHandler.hpp"
#include "Macros.hpp"

class TriggerHandler;
class ActionProcess {
 public:
  ActionProcess(TriggerHandler*, int);
  virtual ~ActionProcess();

  void send_message_to_trigger(trigger_message);

  void callback_function(trigger_message);

  std::shared_ptr<TriggerHandler> trigger_handler;
  int id;
};

#endif /* end of include guard ACTIONPROCESS_HPP */
