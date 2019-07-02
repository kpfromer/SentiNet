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

#include "Publisher.hpp"
#include "ActionProcess.hpp"

int main()
{
	ActionProcess* a = new ActionProcess();

	trigger_message tm = {1, 0, 1};
	trigger_message tm2 = {1, 1, 1};
	trigger_message tm3 = {1, 0, 2};

	Publisher<int> *ab = new Publisher<int>();
	a->publishers[0] = std::shared_ptr<Publisher<int>>(ab);

	int integer = 10;
	a->publishers[0]->publish(&integer);
	a->callback_function(tm);

	return 0;


}
