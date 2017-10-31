#pragma once
#include"TCPClient.h"

bool GuruLib::TCPClient::Connect(const char* ip_addr, unsigned short port)
{
	if (is_socket_initialized)
	{
		struct sockaddr_in _sockaddr_in;
		_sockaddr_in.sin_family = AF_INET;
		_sockaddr_in.sin_port = ::htons(port);
		_sockaddr_in.sin_addr.s_addr = ::inet_addr(ip_addr);

		if (::connect(master_socket, (struct sockaddr *)&_sockaddr_in, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to connect to server. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			master_socket = INVALID_SOCKET;
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool GuruLib::TCPClient::GetIPByDomain(const char* domain, char* ip_addr, unsigned int size)
{
	if (is_socket_initialized)
	{
		struct hostent *he;
		if ((he = gethostbyname(domain)) == NULL)
		{
			//gethostbyname failed
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to get host by domain. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
		}

		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		struct in_addr **addr_list;
		addr_list = (struct in_addr **) he->h_addr_list;
		for (unsigned int i = 0; addr_list[i] != NULL; ++i)
		{
			//Return the first one;
			strcpy_s(ip_addr, size, inet_ntoa(*addr_list[i]));
			break;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool GuruLib::TCPClient::ShutdownConnection()
{
	bool status = true;
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.
	if (::shutdown(master_socket, SD_SEND) < 0)
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
	while (true)
	{
		recv_size = ::recv(master_socket, recv_buffer, ArraySize(recv_buffer), 0);
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
	if (closesocket(master_socket) == SOCKET_ERROR)
	{
		if (handler != NULL)
		{
			sprintf_s(msg, ArraySize(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		status = status && false;
	}
	
	//reset master_socket
	master_socket = INVALID_SOCKET;
	is_socket_initialized = false;

	return status;
}

bool GuruLib::TCPClient::SendString(const char* data, int* bytes_totally_sent)
{
	*bytes_totally_sent = 0;
	int bytes_to_be_sent = (int)strlen(data);
	
	return UnitSendForText(master_socket, data, bytes_totally_sent, bytes_to_be_sent);
}

bool GuruLib::TCPClient::ReceiveString(char* buffer, int length)
{
	//Receive a reply from the server
	recv_size = ::recv(master_socket, buffer, length, 0);
	if (recv_size == SOCKET_ERROR)
	{
#ifdef _DEBUG
		sprintf_s(msg, ArraySize(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
		printf("%s\n", msg);
#endif // _DEBUG
		if (handler != NULL)
		{
			sprintf_s(msg, GuruLib::ArraySize(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	else if (recv_size == 0 || recv_size == WSAECONNRESET)
	{
		if (handler != NULL)
		{
			strcpy_s(msg, ArraySize(msg), "connection closed by peer");
			(*handler)(msg);
		}
		return false;
	}
	else
	{
		//Add a NULL terminating character to make it a proper string before using
		buffer[recv_size] = '\0';
		return true;
	}
}

