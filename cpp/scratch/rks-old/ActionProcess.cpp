/**
 *  @file ActionProcess
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 19:34:34 MDT
 *  @bug No Known Bugs
 */

#include "kernelOPS/core/ActionProcess.hpp"

using namespace obps::core;

ActionProcess::ActionProcess(TriggerHandler* th_, std::string name_) {
  th = std::shared_ptr<TriggerHandler>(th_);
  th->register_action_process(obps::utils::reverse_hash(name_), this);
  asset_name = name_;
}

ActionProcess::~ActionProcess() {}

void ActionProcess::notify_trigger_handler(std::string hash) {
  printf("Im %s and I sent a message to %s\n", asset_name.c_str(),
         hash.c_str());
  notify_trigger_handler(obps::utils::reverse_hash(hash));
}

void ActionProcess::trigger_handler_callback(obps::utils::trigger_message tm) {
  printf("objective %s, I recieved %d %d %d\n", asset_name.c_str(), tm.flag,
         tm.dest, tm.data);
}

void ActionProcess::notify_trigger_handler(obps::utils::trigger_message tm) {
  th->action_process_callback(tm);
}
