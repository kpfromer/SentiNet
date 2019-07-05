/**
 *  @file ServerInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:41:03 MDT
 *  @bug No Known Bugs
 */

#ifndef SERVERINTERFACE_HPP

#define SERVERINTERFACE_HPP

//C++ includes
#include <string>

//Project includes


namespace networking
{
namespace server
{

class ServerInterface
{
    public:
        ServerInterface (const std::string address_);
        virtual ~ServerInterface () = default;

		virtual bool initialize(int) = 0;
		virtual bool terminate() = 0;

    protected:
		const std::string address;
        /* private data */
};

}
}
#endif /* end of include guard SERVERINTERFACE_HPP */

