/**
 *  @file types
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 18:37:08 MDT
 *  @bug No Known Bugs
 */

#ifndef TYPES_HPPI

#define TYPES_HPPI

//C++ includes
#include <iostream>
#include <string>

//Project includes

namespace obps
{
namespace utils
{
	typedef struct
	{
		unsigned char flag:1;
		unsigned char dest:3;
		unsigned char data:4;
	} trigger_message;

	//TODO make this a real hash function
	std::string hash_func(trigger_message* i);
	//Ya this is a really bad hash
	trigger_message reverse_hash(std::string input);
}
}



#endif /* end of include guard TYPES_HPP */

