#pragma once
#include"preinclude.h"

typedef enum
{
	SERVER,
	SUBSERVER,
	REPORTER,
	SUBREPORTER,
	WORKER
}Role;

class ClientSocket;
class ServerSocket;
class NetworkNode
{
private:
	Role role;
	ClientSocket* client_socket;
	ServerSocket* server_socket = NULL;
public:
	NetworkNode();
	virtual ~NetworkNode();
};
