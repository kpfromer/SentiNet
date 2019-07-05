/**
 *  @file ClientInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:24:35 MDT
 *  @bug No Known Bugs
 */

#ifndef CLIENTINTERFACE_HPP

#define CLIENTINTERFACE_HPP

//C++ includes
#include <string>

//Project includes


namespace networking
{
namespace client
{

class ClientInterface
{
    public:
        ClientInterface ();
        virtual ~ClientInterface () = default;

		virtual bool initialize() = 0;
		virtual bool terminate() = 0;

		virtual std::string connect(std::string server_address, std::string, int) = 0;
		virtual bool disconnect(std::string server_address) = 0; 

		virtual std::string request(std::string& server_address, std::string& data) = 0;

};

} 
}

#endif /* end of include guard CLIENTINTERFACE_HPP */

