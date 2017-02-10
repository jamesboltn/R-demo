#pragma once

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
	ServerSocket* server_socket;
public:
	NetworkNode();
	virtual ~NetworkNode();
};
