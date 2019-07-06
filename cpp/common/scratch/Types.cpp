/**
 *  @file utils
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 19:52:25 MDT
 *  @bug No Known Bugs
 */

#include "kernelOPS/utils/Types.hpp"

using namespace obps::utils;

std::string obps::utils::hash_func(trigger_message* i)
{
	char* a = new char[4];
	a[0] = (char)i->flag + 65;
	a[1] = (char)i->dest + 65;
	a[2] = (char)i->data + 65;
	a[3] = (char)0;
	return std::string(a);
}

trigger_message obps::utils::reverse_hash(std::string input)
{
	return {input[0] - 65, input[1] - 65, input[2] - 65};
}
