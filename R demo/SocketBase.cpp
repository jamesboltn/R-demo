#include"SocketBase.h"

TCPPortConnectionState operator|(TCPPortConnectionState a, TCPPortConnectionState b)
{
	return static_cast<TCPPortConnectionState>(static_cast<int>(a) | static_cast<int>(b));
}

bool wsa_startup = false;
bool wsa_cleanup = false;

SocketBase::SocketBase(NetworkProtocol protocol, IPType ip_type):
	handler(NULL),
	sockaddr_in_size(sizeof(struct sockaddr_in))
{
	if (!wsa_startup)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR)
		{
			wsa_startup = true;
		}
	}
	
	if (wsa_startup)
	{
		if ((master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			is_socket_initialized = false;
		}
		else
		{
			is_socket_initialized = true;
		}
	}
	else
	{
		is_socket_initialized = false;
	}
}
SocketBase::SocketBase(SocketErrorMessageHandler _handler, NetworkProtocol protocol, IPType ip_type):
	handler(_handler),
	sockaddr_in_size(sizeof(struct sockaddr_in))
{
	if (!wsa_startup)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "failed to init socket. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
		}
		else
		{
			wsa_startup = true;
		}
	}
	
	if (wsa_startup)
	{
		if ((master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			is_socket_initialized = false;
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "Could not create socket : %d", WSAGetLastError());
				(*handler)(msg);
			}
		}
		else
		{
			is_socket_initialized = true;
		}
	}
	else
	{
		is_socket_initialized = false;
	}
}
SocketBase::~SocketBase()
{
	::closesocket(master_socket);

	if (wsa_startup && !wsa_cleanup)
	{
		::WSACleanup();
		wsa_cleanup = true;
	}
	
}
