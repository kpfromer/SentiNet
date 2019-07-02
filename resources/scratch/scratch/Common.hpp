/**
 *  @file CommonProcesses
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 14:38:02 MDT
 *  @bug No Known Bugs
 */

#ifndef COMMONPROCESSES_HPP

#define COMMONPROCESSES_HPP

//C++ includes

//Project includes

namespace grl
{
namespace processes
{
	typedef enum {zero, low, medium, high} verbosity;

	unsigned int flag_dest(trigger_message);
	unsigned int dest_data(trigger_message);
	unsigned int flag_data(trigger_message);
	unsigned int dest_flag(trigger_message);
	unsigned int data_flag(trigger_message);
	unsigned int data_dest(trigger_message);

	void read_from_action_objective(ActionProcess*, trigger_message);
	void kill(ActionProcess*, trigger_message);
	void start(ActionProcess*, trigger_message);
}
};


#endif /* end of include guard COMMONPROCESSES_HPP */

