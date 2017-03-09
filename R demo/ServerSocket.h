#pragma once
#include"SocketBase.h"

class ServerSocket :
	public SocketBase
{
private:
	bool is_listening_on_port = false;
	bool is_ok;
	std::vector<SOCKET> client_sockets;
	std::vector<SOCKET>::iterator it;
	fd_set read_fds;//set of socket descriptors
	fd_set write_fds;
	SOCKET client_socket;
	bool AcceptConnections();
	bool SendStringData(SOCKET s, const char* data);
	bool ReceiveStringData(SOCKET s, char* buffer, int length);
public:
	ServerSocket();
	bool Bind(unsigned short port);//note that we need not bind to a particular ip since all incoming connections from all network interfaces are acceptable
	bool ShutdownConnection(SOCKET s);//gracefully shutdown the connection
	bool Spinning();
};