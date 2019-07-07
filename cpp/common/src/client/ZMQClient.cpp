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

using namespace networking::client;

ZMQClient::ZMQClient()
	: ClientInterface()
{
}

ZMQClient::~ZMQClient()
{
	terminate();
}

bool ZMQClient::initialize(int context_)
{
	context = ::zmq::context_t(context_);
	socket = std::unique_ptr<::zmq::socket_t>(new ::zmq::socket_t(context, ZMQ_REQ));
	return true;
}

bool ZMQClient::terminate()
{
	return true;
}

bool ZMQClient::connect(const std::string& server_address)
{
	connected_address = server_address;
	return !zmq_connect((void*)(*socket), connected_address.c_str());
}

bool ZMQClient::disconnect(const std::string& server_address)
{
	if(connected_address.empty())
	{
		std::cout<<"Not Connected"<<std::endl;
		return false;
	} 
	bool status = zmq_disconnect((void*)(*socket), connected_address.c_str());
	connected_address = "";
	return !status;
}

bool ZMQClient::make_request(const std::string& request_, std::string& response_)
{
	if(connected_address.empty())
	{
		std::cout<<"Not Connected!"<<std::endl;
		return false;
	}

	::zmq::message_t response;
	::zmq::message_t request(request_.size());
	
	memcpy(request.data(), request_.c_str(), request.size());

	socket->send(request, ::zmq::send_flags::none);
	socket->recv(response, ::zmq::recv_flags::none);

	char* value = (char*)malloc(response.size() + 1);

	memcpy(value, response.data(), response.size() + 1);
	value[response.size()] = '\0'; //TODO theres gotta be a better fix here

	response_ = std::string(value);
	
	return true;
}
