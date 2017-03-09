#pragma once
#include"NetworkNode.h"
#include"ClientSocket.h"
#include"ServerSocket.h"

NetworkNode::NetworkNode():
	client_socket(new ClientSocket())
{
}

NetworkNode::~NetworkNode()
{
	if (client_socket != NULL)
	{
		delete client_socket;
		client_socket = NULL;
	}
	if (server_socket != NULL)
	{
		delete server_socket;
		server_socket = NULL;
	}
}