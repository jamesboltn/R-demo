#pragma once
#include"SocketBase.h"

class ClientSocket :
	public SocketBase
{
private:
	struct in_addr **addr_list;
	struct hostent *he;
	bool ShutdownConnection();//gracefully shutdown the connection
	
public:
	bool Connect(const char* ip_addr, unsigned short port);
	bool TestConnect(const char* ip_addr, unsigned short port);
	bool GetIPByDomain(const char* domain, char* ip_addr);
	bool SendStringData(const char* data);
	bool ReceiveStringData(char* buffer, int length);
};
