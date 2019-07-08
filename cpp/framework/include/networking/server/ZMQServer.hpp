/**
 *  @file ZMQServer
 *  @brief An interface implementation of ServerInterface
 *  
 *  ZMQServer uses ZMQ server socket pattern to emulate a server. It can be single
 *  or multithreaded using some simple commands to make it either one.
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 14:07:20 MDT
 *  @bug No Known Bugs
 */

#ifndef ZMQSERVER_HPP

#define ZMQSERVER_HPP

//C++ includes
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>

//Project includes
#include "networking/server/ServerInterface.hpp"


namespace networking
{
namespace server
{

/**
* @brief A thread entry point
*
* pthread is pretty dumb, so we need a friend entry point that allows us,
* the Server, to enter a new thread while still keeping all inherited traits
* because in the end, we want to execute the function multi_threaded_listener
* when we enter the function, which is private in context (and should be)
*
* @param entry a void pointer entry point that points to this
*
* @return nullptr normally, but used to satisfy pthread_create
*/
void* thread_entry_point(void* entry);

/**
 * @brief Extension of Server Interface
 *
 * A ZMQServer impliments all the ZMQ functionallity (multithreading, piping etc.)
 * to create an idealized server for whatever need. Similar to a publisher, but
 * it listens and responds. For this project, a ZMQServer will reply to a client 
 * pinging it for data (ie requesting a control module)
 */
class ZMQServer : public ServerInterface
{
    public:
		/**
		 * @brief Default constructor
		 *
		 * ZMQConstructor that assignes address to address, maps to
		 * ServerInterface(const std::string&).
		 *
		 * @param address serving_address
		 */
      	ZMQServer (const std::string& address);


		ZMQServer (const std::string& address, int threads);

		ZMQServer (const std::string& address, std::function<std::string(void*, int)> callback);

		ZMQServer (const std::string& address, int threads, std::function<std::string(void*, int)> callback);
		
		
		virtual ~ZMQServer();

		virtual bool initialize(int context = 1);
		virtual bool terminate();
	
		virtual void listen();	

	private:
		void single_threaded_listener();
		void multi_threaded_listener();

		friend void* thread_entry_point(void*);
	

    private:

		::zmq::context_t context;
		std::unique_ptr<::zmq::socket_t> socket;
		int threads;
		
		void* create_entry_point()
		{
			return (void*)this;
		}
		
};

}
}

#endif /* end of include guard ZMQSERVER_HPP */

