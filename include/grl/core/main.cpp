/**
 *  @file main
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 19:12:10 MDT
 *  @bug No Known Bugs
 */

#include "TriggerHandler.hpp"
#include "Types.hpp"



using namespace grl::processes;
int main()
{
	TriggerHandler* th = new TriggerHandler();
	ActionProcess* a = new ActionProcess( th, "charlie");
	ActionProcess* b = new ActionProcess( th, "scott");
	ActionProcess* c = new ActionProcess( th, "henry");
	ActionProcess* d = new ActionProcess( th, "chumps");

	a->notify_trigger_handler("scott");
	b->notify_trigger_handler("chumps");
	c->notify_trigger_handler("henry");
	d->notify_trigger_handler("charlie");
	return 0;
}
