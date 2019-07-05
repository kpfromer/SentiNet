/**
 *  @file ZMQClient
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:44:13 MDT
 *  @bug No Known Bugs
 */

#ifndef ZMQCLIENT_HPP

#define ZMQCLIENT_HPP

//C++ includes
#include <zmq.hpp>
#include <memory>
#include <unordered_map>
#include <string>

//Project includes
#include "networking/client/ClientInterface.hpp"

namespace networking
{
namespace client
{

class ZMQClient : public ClientInterface
{
    public:
        ZMQClient ();
         ~ZMQClient ();

		 virtual bool initialize();
		 virtual bool terminate();

		 virtual std::string connect(std::string server_address, std::string request, int context = 1);
		 virtual bool disconnect(std::string server_address);

		 virtual std::string request(std::string& server_address, std::string& data);

    private:

		std::unordered_map<std::string, std::unique_ptr<::zmq::socket_t>> servers;

		::zmq::context_t context;
};

}
}
#endif /* end of include guard ZMQCLIENT_HPP */

