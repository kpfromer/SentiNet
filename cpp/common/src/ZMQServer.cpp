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

typedef struct
{
	ZMQServer * server;
	::zmq::context_t * context;
} context_world;

void* networking::server::thread_entry_point(void* obj)
{
	std::cout<<"entering thread"<<std::endl;
	context_world* thr = ((context_world*)obj);
	thr->server->multi_threaded_listener((void*)thr->context);
	return nullptr;
}

ZMQServer::ZMQServer(const std::string address_, int context_)
	: ServerInterface(address_)
{
	context = context_;
}

ZMQServer::~ZMQServer()
{
}

bool ZMQServer::initialize(int threads)
{
	if(threads > 1)
	{
		::zmq::context_t context_(context);
		::zmq::socket_t clients(context_, ZMQ_ROUTER);
		clients.bind(address);
		zmq::socket_t workers(context_, ZMQ_DEALER);
		workers.bind("inproc://workers");

		//launch worker threads
		for(int i = 0; i < threads; i++)
		{
			pthread_t worker;
			
			context_world* world = new context_world;
			world->server = this;
			world->context = &context_;

			pthread_create(&worker, NULL, thread_entry_point, (void*)world);
		}

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
			socket.recv(request);
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
		socket.recv(request);
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



