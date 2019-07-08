/**
 *  @file TriggerHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 15:55:58 MDT
 *  @bug No Known Bugs
 */

#include "TriggerHandler.hpp"

void TriggerHandler::test2(trigger_message tm) {
  printf("Im a trigger and I recieved %d, %d, %d\n", tm.flag, tm.dest, tm.data);

  messages.push(tm);

  printf("id = %d, %d\n", tm.dest, aps[tm.dest]->id);
  aps[tm.dest]->callback_function(tm);
}

TriggerHandler::TriggerHandler() {}

TriggerHandler::~TriggerHandler() {}

void TriggerHandler::add_ap(ActionProcess* ap) { aps.push_back(ap); }

void TriggerHandler::output_trigger() {
  while (!messages.empty()) {
    printf("Im a trigger and I have %d, %d, %d\n", messages.front().flag,
           messages.front().dest, messages.front().data);
    messages.pop();
  }
}
