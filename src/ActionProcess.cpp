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



ActionProcess::ActionProcess()
{
}

ActionProcess::~ActionProcess()
{
}

void ActionProcess::callback_function(trigger_message tm)
{
	int i = 5;
	publishers[0]->publish(&i);
}
