#pragma once
#include"UDPServer.h"
#include"NetworkGlobalObject.h"

//TODO::port SendStringData of server version to client version
bool GuruLib::UDPServer::SendString(SOCKET s, const char* data, int* bytes_totally_sent)
{
	*bytes_totally_sent = 0;
	int bytes_to_be_sent = (int)strlen(data);

	int bytes_unit_sent;
	int udp_send_circuits = 0;
	bool no_error = true;
	while (bytes_to_be_sent > udp_send_max_msg_size)
	{
		no_error = no_error&&UnitSendForText(s, &data[udp_send_circuits], &bytes_unit_sent, udp_send_max_msg_size);
		*bytes_totally_sent += bytes_unit_sent;
		bytes_to_be_sent -= udp_send_max_msg_size;
		udp_send_circuits += udp_send_max_msg_size;
	}

	no_error = no_error&&UnitSendForText(s, &data[udp_send_circuits], &bytes_unit_sent, bytes_to_be_sent);
	*bytes_totally_sent += bytes_unit_sent;
	return no_error;
}

void GuruLib::UDPServer::Listen(unsigned short port)
{
#pragma region bind your server to port
	if (is_socket_initialized)
	{
		//Prepare the sockaddr_in structure
		struct sockaddr_in _sockaddr_in;
		_sockaddr_in.sin_family = AF_INET;
		_sockaddr_in.sin_addr.s_addr = INADDR_ANY;
		_sockaddr_in.sin_port = ::htons(port);

		//Bind
		if (::bind(master_socket, (struct sockaddr *)&_sockaddr_in, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to bind socket to port. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return;
		}
	}
	else
	{
		return;
	}
#pragma endregion

	while (true)
	{
		//waiting for connections
		AcceptConnections();
		//Acceptor restarting...
	}
}

GuruLib::UDPServer::~UDPServer()
{
	if (is_socket_initialized)
	{
		::closesocket(master_socket);
	}
}

GuruLib::UDPServer::UDPServer():
	ServerBase(this)
{
#ifdef STATIC_LINK
	if (wsa_startup)
#else
	if (NetworkGlobal::wsa_startup)
#endif // STATIC_LINK
	{
		master_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		int optlen = sizeof(int);
		if (::getsockopt(master_socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *)&udp_send_max_msg_size, &optlen) == SOCKET_ERROR)
		{
			master_socket = INVALID_SOCKET;
		}

		if (master_socket == INVALID_SOCKET)
		{
			is_socket_initialized = false;
			if (handler != NULL)
			{
				sprintf_s(msg, GuruLib::ArraySize(msg), "Could not create socket : %d", WSAGetLastError());
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

bool GuruLib::UDPServer::ShutdownConnection(SOCKET s)
{
	bool status = true;
	
	//TODO:write UDP partial disconnection logic here

	// Close the socket.
	if (closesocket(s) == SOCKET_ERROR) {
		if (handler != NULL)
		{
			sprintf_s(msg, GuruLib::ArraySize(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		status = status && false;
	}

	s = INVALID_SOCKET;

	return status;	
}
