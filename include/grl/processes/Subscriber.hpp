/**
 *  @file Subscriber
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 15:14:28 MDT
 *  @bug No Known Bugs
 */

#ifndef SUBSCRIBER_HPP

#define SUBSCRIBER_HPP

//C++ includes

//Project includes
class SubscriberBase
{
	public:
		virtual ~SubscriberBase();
		std::string get_name() const;
		
	private:
		const std::string name;
}


template<typename message>
class Subscriber : public SubscriberBase
{
    public:
        Subscriber ();
        virtual ~Subscriber ();

		message get_data();	
    private:
		std::shared_ptr<ObjectiveHandler> handler;
        /* private data */
};

#endif /* end of include guard SUBSCRIBER_HPP */

