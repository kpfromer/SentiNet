/**
 *  @file ServerInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:11:02 MDT
 *  @bug No Known Bugs
 */

#include "networking/server/ServerInterface.hpp"
#include <iostream>

using namespace networking::server;

ServerInterface::ServerInterface(const std::string& address_)
	: serving_address(address_)
{
	callback = std::unique_ptr<std::function<std::string(void*, int)>>
		(new std::function<std::string(void*, int)>
		 [this](void* data, int size) -> std::string
	{
		char* value = (char*)malloc(size);
		memcpy(value, data, size);
		value[size] = '\0';
		std::string val = std::string(value);
		free(value);
		return val;
	});
	if(callback == nullptr)
	{
		std::cout<<"no"<<std::endl;
	}else{
	std::cout<<"Im here"<<std::endl;
}
}

ServerInterface::ServerInterface(const std::string& add, std::function<std::string(void*, int)> callback_)
	: serving_address(add)
{
	set_callback(callback_);
}
