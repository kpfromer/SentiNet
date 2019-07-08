/**
 *  @file ObjectiveHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 19:57:46 MDT
 *  @bug No Known Bugs
 */

#include "kernelOPS/core/ObjectiveHandler.hpp"

using namespace obps::core;

ObjectiveHandler::ObjectiveHandler()
{
}

ObjectiveHandler::~ObjectiveHandler()
{
}

void ObjectiveHandler::add_objective(ActionObjective* a, std::string name)
{	
	objectives[name] = std::shared_ptr<ActionObjective>(a);
}	

std::shared_ptr<ActionObjective> ObjectiveHandler::get_objective(std::string name)
{
	return objectives[name];
}
