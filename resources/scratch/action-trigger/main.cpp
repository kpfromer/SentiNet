/**
 *  @file main
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 16:03:29 MDT
 *  @bug No Known Bugs
 */

#include "TriggerHandler.hpp"
#include "ActionProcess.hpp"

int main()
{
	TriggerHandler* th = new TriggerHandler();
	ActionProcess* a = new ActionProcess(th, 0);
	ActionProcess* b = new ActionProcess(th, 1);
	th->add_ap(a);
	th->add_ap(b);

	trigger_message tm = {1, 0, 1};
	trigger_message tm2 = {1, 1, 1};
	trigger_message tm3 = {1, 0, 2};
	
	a->send_message_to_trigger(tm);
	a->send_message_to_trigger(tm2);
	a->send_message_to_trigger(tm3);
	th->output_trigger();

	return 0;


}
