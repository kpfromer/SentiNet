/**
 *  @file ZMQClient
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 13:55:49 MDT
 *  @bug No Known Bugs
 */

#include "networking/client/ZMQClient.hpp"
#include <iostream>
#include <thread>
#include <stdio.h>

using namespace networking::client;

ZMQClient::ZMQClient()
	: ClientInterface()
{
}

ZMQClient::~ZMQClient()
{
}

bool ZMQClient::initialize()
{
	return true;
}

bool ZMQClient::terminate()
{
	return true;
}

std::string ZMQClient::connect(std::string server_address,  std::string request_, int context_)
{
	::zmq::context_t context(context_);
	::zmq::socket_t * socket = new ::zmq::socket_t(context, ZMQ_REQ);
	servers[server_address] = std::unique_ptr<::zmq::socket_t>(socket);	
	servers[server_address]->connect(server_address);
	std::string response = request(server_address, request_);
	disconnect(server_address);
	return response;
}

bool ZMQClient::disconnect(std::string server_address)
{
	return servers.erase(server_address);
}

std::string ZMQClient::request(std::string& server_address, std::string& request)
{
	::zmq::message_t response;
	::zmq::message_t request_(request.size());
	memcpy(request_.data(), request.c_str(), request.size());
	servers[server_address]->send(request_);
	servers[server_address]->recv(&response, 0);
	return std::string((char*)response.data());
}

