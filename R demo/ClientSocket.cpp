#pragma once
#include"ClientSocket.h"

bool ClientSocket::Connect(const char* ip_addr, unsigned short port)
{
	if (is_socket_initialized)
	{
		_sockaddr_in.sin_addr.s_addr = ::inet_addr(ip_addr);
		_sockaddr_in.sin_family = AF_INET;
		_sockaddr_in.sin_port = ::htons(port);

		if (::connect(master_socket, (struct sockaddr *)&_sockaddr_in, sockaddr_in_size) < 0)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "failed to connect to server. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}
static SOCKET test_s;
bool ClientSocket::TestConnect(const char* ip_addr, unsigned short port)
{
	if (is_socket_initialized)
	{
		if ((test_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != INVALID_SOCKET)
		{
			_sockaddr_in.sin_addr.s_addr = ::inet_addr(ip_addr);
			_sockaddr_in.sin_family = AF_INET;
			_sockaddr_in.sin_port = ::htons(port);

			if (::connect(test_s, (struct sockaddr *)&_sockaddr_in, sockaddr_in_size) < 0)
			{
				closesocket(test_s);
				return false;
			}
			else
			{
				::shutdown(test_s, SD_BOTH);
				::closesocket(test_s);
				return true;
			}
		}
		else
		{
			closesocket(test_s);
			return false;
		}
	}
	else
	{
		return false;
	}

		
	
}
bool ClientSocket::GetIPByDomain(const char* domain, char* ip_addr)
{
	if (is_socket_initialized)
	{
		if ((he = gethostbyname(domain)) == NULL)
		{
			//gethostbyname failed
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "failed to get host by domain. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
		}

		//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;
		for (int i = 0; addr_list[i] != NULL; ++i)
		{
			//Return the first one;
			strcpy_s(ip_addr, ARRAYCOUNT(ip_addr), inet_ntoa(*addr_list[i]));
			break;
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool ClientSocket::ShutdownConnection()
{
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.
	if (shutdown(master_socket, SD_SEND) < 0)
	{
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to shutdown connection. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}

	// Receive any extra data still sitting on the socket.  After all
	// data is received, this call will block until the remote host
	// acknowledges the TCP control packet sent by the shutdown above.
	// Then we'll get a 0 back from recv, signalling that the remote
	// host has closed its side of the connection.
	while (1) {
		recv_size = ::recv(master_socket, recv_buffer, ARRAYCOUNT(recv_buffer), 0);
		if (recv_size < 0)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
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
	if (closesocket(master_socket) == SOCKET_ERROR) {
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}

	return true;
}
bool ClientSocket::SendStringData(const char* data)
{
	recv_size = ::send(master_socket, data, (int)strlen(data), 0);
	if ( recv_size < 0)
	{
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to send string data to server. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	return true;
}
bool ClientSocket::ReceiveStringData(char* buffer, int length)
{
	//Receive a reply from the server
	recv_size = (recv_size = ::recv(master_socket, buffer, length - 1, 0));
	if (recv_size < 0)
	{
#ifdef _DEBUG
		sprintf_s(msg, ARRAYCOUNT(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
		printf("%s\n", msg);
#endif // _DEBUG
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	else if (recv_size == 0)
	{
		if (handler != NULL)
		{
			strcpy_s(msg, ARRAYCOUNT(msg), "connection reset by peer");
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
