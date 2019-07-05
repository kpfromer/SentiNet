/**
 *  @file Clienttest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:45:27 MDT
 *  @bug No Known Bugs
 */

#include "networking/client/ZMQClient.hpp"
#include <iostream>

using namespace networking::client;
int main()
{
	ZMQClient* a = new ZMQClient();

	for(int i = 0; i < 10; i++)
	{
		a->connect("tcp://localhost:5555", "hello");
	}

	return 0;
}
