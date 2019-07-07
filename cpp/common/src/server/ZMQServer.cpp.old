/**
 *  @file ZMQServer
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 17:12:06 MDT
 *  @bug No Known Bugs
 */

#include "networking/server/ZMQServer.hpp"
#include <iostream>
#include <unistd.h>

using namespace networking::server;


/*
 * We need a thread entry function to capture <this> because pthread only accepts a void pointer
 * to "context" or whatever else the thread needs. Thus, the entry point is a friend to ZMQServer,
 * it can access private vars and functions. It's essentially the same as the object
 */
//obj is { this, void* (context) }
void* networking::server::thread_entry_point(void* obj)
{
  //extract this from obj
	::callbacks::entry_context<ZMQServer>* thr = ((::callbacks::entry_context<ZMQServer>*)obj);

  //start main process of execution
	thr->object->multi_threaded_listener(thr->context);

  //make sure to delete context - or else it wont be deleted until the end remember, its 
  //a void pointer right now.
  delete ((::zmq::context_t*)thr->context);

  //mak the compiler happy
	return nullptr;
}

//Initialize a server with a context
ZMQServer::ZMQServer(const std::string address_, int context_)
	: ServerInterface(address_)
{
	context = context_;
}

//delete server
ZMQServer::~ZMQServer()
{
}

//do work with a set amount of threads
bool ZMQServer::initialize(int threads)
{
  //If we want multithreading, we need to utilize zmq's in process
  //communication, otherwise, zmq is a straight shot that has no idea what to do
	if(threads > 1)
	{
    //create a new context - scoped here - deleted as we leave TODO change to class attribute
		::zmq::context_t context_(context);
		::zmq::socket_t clients(context_, ZMQ_ROUTER);

    //bind to self address - this is the master socket that accepts incomming data from workers
		clients.bind(address);

    //create a new worker socket - these accept incomming messages and pipe them to the main server
		zmq::socket_t workers(context_, ZMQ_DEALER);
    //special type of transport protocol
		workers.bind("inproc://workers");

		//launch worker threads
		for(int i = 0; i < threads; i++)
		{
			pthread_t worker;	
      //create an entry point to the thread, remember, pthread is pretty dumb, it needs special priveledges
      //and access to class contents
      void* entry_point = ::callbacks::create_entry(this, (void*)&context_);
			pthread_create(&worker, NULL, thread_entry_point, entry_point);
		}
    
    //create a zmq proxy that listens to workers
    //proxy connects frontend sockets to backend sockets
		zmq::proxy(clients, workers, NULL);
	} else {
		single_threaded_listener();
	}
	return true;
}

bool ZMQServer::terminate()
{
	return true;
}

void ZMQServer::single_threaded_listener()
{	
		//create the context and socket in the process so their deleted on exit
		::zmq::context_t context_(context);
		::zmq::socket_t socket(context_, ZMQ_REP);

		//bind to address
		socket.bind(address);

		while(true) 
		{
			//placeholder for request
			zmq::message_t request;

			std::cout<<"Waiting for client ..."<<std::endl;
			socket.recv(&request, 0);
			std::cout<<"Got A message from client .... "<<std::endl;

			//"do work"
			std::string result = process(request.data());

			//reply 
			::zmq::message_t reply(result.size());
			memcpy(reply.data(), result.c_str(), result.size());
			socket.send(reply);
		}
}

void* ZMQServer::multi_threaded_listener(void* arg)
{
	::zmq::context_t *context_ = (::zmq::context_t*)arg;	
	::zmq::socket_t socket(*context_, ZMQ_REP);
	socket.connect("inproc://workers"); //really the only difference
	
	while(true)
	{
		zmq::message_t request;

		std::cout<<"Waiting for client...."<<std::endl;
		socket.recv(&request, 0);
		std::cout<<"Got a message from client ...."<<std::endl;

		//"do work"
		std::string result = process(request.data());

		::zmq::message_t reply(result.size());
		memcpy((void*)reply.data(), result.c_str(), 6);
		socket.send(reply);
	}
	return nullptr;
}

std::string ZMQServer::process(void* arg)
{
	std::string value((char*)arg);
	sleep(1); //do work
	return value;
}



