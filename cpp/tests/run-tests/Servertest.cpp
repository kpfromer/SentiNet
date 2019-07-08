/**
 *  @file Servertest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:43:15 MDT
 *  @bug No Known Bugs
 */

#include "networking/server/ZMQServer.hpp"
#include <iostream>

using namespace networking::server;

int main()
{

	ZMQServer* a = new ZMQServer("tcp://*:5555", 1);

	a->initialize();
	a->listen();
	a->terminate();

	return 0;
}
