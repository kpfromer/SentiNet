/**
 *  @file ZMQServer
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
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
#include "callbacks/callbacks.hpp"


namespace networking
{
namespace server
{

void* thread_entry_point(void*);

class ZMQServer : public ServerInterface
{
    public:
      ZMQServer (const std::string address_, int context_ = 1);
      ZMQServer (int port);
      virtual ~ZMQServer ();

		  virtual bool initialize(int threads = 1);
		  virtual bool terminate();
	
		 

    private:

		  void* multi_threaded_listener(void*);

		  friend ::callbacks::entry_func thread_entry_point; 

		  void single_threaded_listener();

		  std::string process(void*);

		  int context;
		  std::unique_ptr<::zmq::socket_t> socket;

		
};

}
}

#endif /* end of include guard ZMQSERVER_HPP */

