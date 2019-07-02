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

//Project includes
#include "grl/macros.h"
#include "grl/processes/ActionObjective.hpp"
#include "grl/processes/TriggerObjective.hpp"


namespace grl
{
namespace processes
{

/**
 *	A Base class for Action Process with default methods 
 *
 *	Action Process Interface is a base class that allows 
 *	inheriting classes to have default constructing behavior
 *	Action processes have a list of action objectives they
 *	write to and read to as well as a trigger objective that communicates
 *	with other action processes
 */
class ActionProcess
{
    public:
		/**
		 *	Default Constructor
		 *
		 *	This creates an Action Process Interface with null pointers as
		 *	action objectives and action id 
		 */
        ActionProcess ();
		virtual ~ActionProcess ();

	public:
		void add_subscriber(SubscriberBase*);
		void add_publisher(PublisherBase*);

		//Action process -> trigger
		void send_message_to_trigger(trigger_message);


	private:
		/** A lock for when reading from action objective */
		std::mutex lock;			

		//Trigger -> action proces
		void (*trigger_callback)(trigger_message);

		std::vector<std::shared_ptr<PublisherBase>> publishers;
		std::vector<std::shared_ptr<PublisherBase>> subscribers;

		std::shared_ptr<TriggerHandler> trigger_handler;
		std::shared_ptr<ObjectiveHandler> objective_handler;
};
}	//namespace processes
}	//namespace grl

#endif /* end of include guard ACTIONPROCESS_HPP */

