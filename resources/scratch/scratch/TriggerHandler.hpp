/**
 *  @file TriggerHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 13:53:21 MDT
 *  @bug No Known Bugs
 */

#ifndef TRIGGERHANDLER_HPP

#define TRIGGERHANDLER_HPP

//C++ includes
#include <memory>
#include <vector>

//Project includes
#include "grl/processes/ActionObjective.hpp"
#include "grl/processes/ActionProcess.hpp"

namespace grl
{
namespace processes
{


/**
 * @brief 
 */
class TriggerHandler
{
    public:
        TriggerHandler ();
        virtual ~TriggerHandler ();
		
	private:
		// action process -> trigger
		void (*input_trigger)(trigger_message); //!< Mutex protected

		// trigger -> action_process
		void output_trigger(trigger_message); // TODO trigger_messages should have a lot of common functions that allow ease of use

		/**
		 * @brief Child action processes
		 */
		std::vector<std::unique_ptr<ActionProcess>> processes;
		std::shared_ptr<std::vector<std::function<void* (ActionProcess*, trigger_message)>>>; //!< shared because there could be more than one trigger handlers 
		std::queue<trigger_message> messages;

};

} 	//namespace processes
}	//namespace grl

#endif /* end of include guard TRIGGERHANDLER_HPP */

