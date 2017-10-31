#pragma once
#include"TCPServer.h"
#include"NetworkGlobalObject.h"

//TODO::port SendStringData of server version to client version
bool GuruLib::TCPServer::SendString(SOCKET s, const char* data, int* bytes_totally_sent)
{
	*bytes_totally_sent = 0;
	int bytes_to_be_sent = (int)strlen(data);

	return UnitSendForText(s, data, bytes_totally_sent, bytes_to_be_sent);
}

void GuruLib::TCPServer::Listen(unsigned short port)
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

		//only TCP need to do extra listen
		if (::listen(master_socket, 3) == SOCKET_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to place socket in listening state. error code : %d", WSAGetLastError());
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

GuruLib::TCPServer::~TCPServer()
{
	if (is_socket_initialized)
	{
		::closesocket(master_socket);
	}
}

GuruLib::TCPServer::TCPServer():
	ServerBase(this)
{
#ifdef STATIC_LINK
	if (wsa_startup)
#else
	if (NetworkGlobal::wsa_startup)
#endif // STATIC_LINK
	{
		master_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (master_socket == INVALID_SOCKET)
		{
			is_socket_initialized = false;
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "Could not create socket : %d", WSAGetLastError());
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

bool GuruLib::TCPServer::ShutdownConnection(SOCKET s)
{
	bool status = true;
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.
	if (::shutdown(s, SD_SEND) < 0)
	{
		if (handler != NULL)
		{
			::sprintf_s(msg, ArraySize(msg), "failed to disable send() in order to shutdown connection. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		status = status && false;
	}

	// Receive any extra data still sitting on the socket.  After all
	// data is received, this call will block until the remote host
	// acknowledges the TCP control packet sent by the shutdown above.
	// Then we'll get a 0 back from recv, signalling that the remote
	// host has closed its side of the connection.
	while (true) {
		recv_size = ::recv(s, recv_buffer, ArraySize(recv_buffer), 0);
		if (recv_size < 0)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			status = status && false;
			break;
		}
		else if (recv_size > 0) {
			//received send_recv_size unexpected bytes during shutdown.
		}
		else {
			// Okay, we're done!
			break;
		}
	}

	// Close the socket.
	if (closesocket(s) == SOCKET_ERROR) {
		if (handler != NULL)
		{
			sprintf_s(msg, ArraySize(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		status = status && false;
	}

	s = INVALID_SOCKET;

	return status;
}


