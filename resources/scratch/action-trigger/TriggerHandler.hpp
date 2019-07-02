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
#include <iostream>
#include <queue>

//Project includes
#include "Macros.hpp"
#include "ActionProcess.hpp"

class ActionProcess;

class TriggerHandler
{
    public:
        TriggerHandler ();
        virtual ~TriggerHandler ();
		void test2(trigger_message);	
		//void (TriggerHandler::*input_trigger)(trigger_message); //!< Mutex protected
		void add_ap(ActionProcess* ap);

		void output_trigger(); // 
		std::queue<trigger_message> messages;
		std::vector<ActionProcess*> aps;

};

#endif /* end of include guard TRIGGERHANDLER_HPP */

