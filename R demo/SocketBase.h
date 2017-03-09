#pragma once
#include"preinclude.h"

typedef enum
{
	TCP,
	UDP
}NetworkProtocol;
typedef enum
{
	IPv4,
	IPv6
}IPType;
typedef void(*SocketErrorMessageHandler)(const char* msg);
typedef enum
{
	TESTING,
	CONNECTABLE,
	UNCONNECTABLE
}TCPPortConnectionState;
TCPPortConnectionState operator|(TCPPortConnectionState a, TCPPortConnectionState b);
class SocketBase
{
private:
	WSADATA wsa;
protected:
	char msg[100];
	SocketErrorMessageHandler handler;
	bool is_socket_initialized;
	struct sockaddr_in _sockaddr_in;
	int sockaddr_in_size;
	SOCKET master_socket;
public:
	char recv_buffer[2000];
	int recv_size;
	SocketBase(NetworkProtocol protocol = TCP, IPType ip_type = IPv4);
	SocketBase(SocketErrorMessageHandler _handler, NetworkProtocol protocol = TCP, IPType ip_type = IPv6);
	virtual ~SocketBase();
};