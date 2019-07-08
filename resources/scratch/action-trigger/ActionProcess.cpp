/**
 *  @file ActionProcess
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 15:46:02 MDT
 *  @bug No Known Bugs
 */

#include "ActionProcess.hpp"

ActionProcess::ActionProcess(TriggerHandler* th, int id_) {
  trigger_handler = std::shared_ptr<TriggerHandler>(th);
  id = id_;
}

ActionProcess::~ActionProcess() {}

void ActionProcess::send_message_to_trigger(trigger_message tm) {
  trigger_handler->test2(tm);
}

void ActionProcess::callback_function(trigger_message tm) {
  printf("Action %d I recieved %d %d %d\n", id, tm.flag, tm.dest, tm.data);
}
