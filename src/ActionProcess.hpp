/**
 *  @file ActionProcess
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jun 29, 2019 14:19:31 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTIONPROCESS_HPP

#define ACTIONPROCESS_HPP


//C++ includes
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>

//Project includes
#include "Publisher.hpp"
#include "Macros.hpp"

class ActionProcess
{
    public:
        ActionProcess ();
		virtual ~ActionProcess ();

		void callback_function(trigger_message);

		std::vector<std::shared_ptr<PublisherBase>> publishers;
		int id;
};

#endif /* end of include guard ACTIONPROCESS_HPP */

