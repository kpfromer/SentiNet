/**
 *  @file ActionProcessInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 21:27:14 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTIONPROCESSINTERFACE_HPP

#define ACTIONPROCESSINTERFACE_HPP

//C++ includes

namespace obps
{
namespace core
{

class ActionProcessInterface
{
    public:
        ActionProcessInterface (TriggerHandler* th_, const std::string name);
		
        virtual ~ActionProcessInterface ();

		virtual void notify_trigger_handler(const std::string hash);
		virtual void notify_trigger_handler(obps::utils::trigger_message tm);

		virtual void trigger_handler_callback(obps::utils::trigger_message tm);

    private:
		virtual 

		std::shared_ptr<TriggerHandlerInterface> th;
		std::shared_ptr<ObjectiveHandlerInterface> oh

        /* private data */
};

#endif /* end of include guard ACTIONPROCESSINTERFACE_HPP */

