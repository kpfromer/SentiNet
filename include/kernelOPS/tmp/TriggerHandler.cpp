/**
 *  @file TriggerHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 19:40:42 MDT
 *  @bug No Known Bugs
 */

#include "TriggerHandler.hpp"

using namespace obps::core;

TriggerHandler::TriggerHandler()
{ 
}

TriggerHandler::~TriggerHandler()
{
}

void TriggerHandler::register_action_process(obps::utils::trigger_message tm, ActionProcess* a)
{
	std::string hash = obps::utils::hash_func(&tm);
	children[hash] = std::shared_ptr<ActionProcess>(a);
}

void TriggerHandler::action_process_callback(obps::utils::trigger_message tm)
{
	message_queue.push(tm);
	notify_action_process(tm);
	message_queue.pop();
}

void TriggerHandler::notify_action_process(obps::utils::trigger_message tm)
{
	lock.lock();
	children[obps::utils::hash_func(&tm)]->trigger_handler_callback(tm);
	lock.unlock();
}

