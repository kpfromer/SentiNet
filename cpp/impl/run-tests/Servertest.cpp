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
	auto callback = [](void* context, int size) -> std::string
	{
		std::cout<<"in"<<std::endl;
		char* value = (char*)malloc(size);
		memcpy(value, context, size);
		value[size] = '\0';
		std::string val = std::string(value);
		val += " response";
		free(value);
		return val;
	};

	ZMQServer* a = new ZMQServer("tcp://*:5555", 1, callback);


	a->initialize();
	a->listen();
	a->terminate();

	return 0;
}
