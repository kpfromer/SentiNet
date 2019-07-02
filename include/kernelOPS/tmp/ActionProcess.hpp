/**
 *  @file ActionProcess
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 18:35:21 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTIONPROCESS_HPP

#define ACTIONPROCESS_HPP

//C++ includes
#include <iostream>
#include <string>
#include <memory>

//Project includes
#include "Types.hpp"
#include "TriggerHandler.hpp"

namespace obps
{
namespace core
{
class TriggerHandler;
class ActionProcess
{
    public:
        ActionProcess (TriggerHandler* th_, std::string name);
        virtual ~ActionProcess ();

		void notify_trigger_handler(std::string hash);
		void notify_trigger_handler(obps::utils::trigger_message tm);

	
		void trigger_handler_callback(obps::utils::trigger_message tm);

	private:
		std::shared_ptr<TriggerHandler> th;
		std::string asset_name;
};
}
}

#endif /* end of include guard ACTIONPROCESS_HPP */

