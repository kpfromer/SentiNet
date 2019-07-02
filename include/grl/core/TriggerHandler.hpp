/**
 *  @file TriggerHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 18:18:32 MDT
 *  @bug No Known Bugs
 */

#ifndef TRIGGERHANDLER_HPP

#define TRIGGERHANDLER_HPP

//C++ includes
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

//Project includes
#include "ActionProcess.hpp"
#include "Types.hpp"



namespace grl
{
namespace processes
{
class ActionProcess;
class TriggerHandler
{
    public:
        TriggerHandler ();
        virtual ~TriggerHandler ();

		void register_action_process(grl::utils::trigger_message tm, ActionProcess* a);
		void action_process_callback(grl::utils::trigger_message tm);
		
	private:
		//void notify_action_objective_table(::utils::trigger_message)
		void notify_action_process(grl::utils::trigger_message tm);

		std::unordered_map<std::string, std::shared_ptr<ActionProcess>> children;
		std::queue<grl::utils::trigger_message> message_queue;
		std::mutex lock;

};

}
}
#endif /* end of include guard TRIGGERHANDLER_HPP */

