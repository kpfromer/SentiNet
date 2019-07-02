/**
 *  @file Publisher
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 15:10:50 MDT
 *  @bug No Known Bugs
 */

#ifndef PUBLISHER_HPP

#define PUBLISHER_HPP

//C++ includes
#include <iostream>
//Project includes



class PublisherBase
{
	public:
		virtual ~PublisherBase() = default;

		virtual void publish(void*) = 0;
	private:
};
template <typename message>
class Publisher : public PublisherBase
{
    public:
        Publisher (){}
        ~Publisher (){}

	public:
		void publish(void* t){std::cout<<*(message*)t<<std::endl;}

    private:
};

#endif /* end of include guard PUBLISHER_HPP */

