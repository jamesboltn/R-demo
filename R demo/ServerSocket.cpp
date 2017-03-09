#pragma once
#include"ServerSocket.h"

ServerSocket::ServerSocket()
{
}

bool ServerSocket::Bind(unsigned short port)
{
	if (is_socket_initialized)
	{
		//Prepare the sockaddr_in structure
		_sockaddr_in.sin_family = AF_INET;
		_sockaddr_in.sin_addr.s_addr = INADDR_ANY;
		_sockaddr_in.sin_port = htons(port);

		//Bind
		if (::bind(master_socket, (struct sockaddr *)&_sockaddr_in, sockaddr_in_size) < 0)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ARRAYCOUNT(msg), "failed to bind socket to port. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
		}
		else
		{
			//Listen
			if (::listen(master_socket, 3) < 0)
			{
				if (handler != NULL)
				{
					sprintf_s(msg, ARRAYCOUNT(msg), "failed to place socket in listening state. error code : %d", WSAGetLastError());
					(*handler)(msg);
				}
				return false;
			}
			is_listening_on_port = true;
			return true;
		}
	}
	else
	{
		return false;
	}
	
} 

//TODO::port SendStringData of server version to client version
bool ServerSocket::SendStringData(SOCKET s, const char* data)
{
	recv_size = ::send(s, data, (int)strlen(data), 0);
	if (recv_size < 0)
	{
#ifdef _DEBUG
		sprintf_s(msg, ARRAYCOUNT(msg), "failed to send string data to client. error code : %d", WSAGetLastError());
		printf("%s\n", msg);
#endif // _DEBUG
		// Something bad happened on the socket.  Deal with it.
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to send string data to peer. error code : %d", WSAGetLastError());
				(*handler)(msg);
		}
		return false;
	}
	return true;
}
bool ServerSocket::ReceiveStringData(SOCKET s, char* buffer, int length)
{
	//get details of the client
	//getpeername(*it, (struct sockaddr*)&_sockaddr_in, (int*)&sockaddr_in_size);
	//Somebody disconnected , get his details and print
	//printf("Host disconnected , ip %s , port %d \n", inet_ntoa(_sockaddr_in.sin_addr), ntohs(_sockaddr_in.sin_port));
	
	recv_size = ::recv(s, buffer, length - 1, 0);
	if (recv_size < 0)
	{
		// Something bad happened on the socket.  It could just be a
		// "would block" notification, or it could be something more
		// serious.  Let caller handle the latter case.  WSAEWOULDBLOCK
		// can happen after select() says a socket is readable under
		// Win9x: it doesn't happen on WinNT/2000 or on Unix.
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
		//Somebody disconnected
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
bool ServerSocket::AcceptConnections()
{
	if (is_listening_on_port)
	{
		//clear the socket fd set
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		// Add the listener socket to the read FD sets
		FD_SET(master_socket, &read_fds);//add master socket to fd set

		while (::select(0, &read_fds, &write_fds, NULL, NULL) >= 0)//wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
		{
#pragma region something happened on the master socket, an incoming connection
			if (FD_ISSET(master_socket, &read_fds))
			{
				if ((client_socket = accept(master_socket, (struct sockaddr*)&_sockaddr_in, (int*)&sockaddr_in_size)) == INVALID_SOCKET)
				{
					if (handler != NULL)
					{
						sprintf_s(msg, ARRAYCOUNT(msg), "accept() call failed. error code : %d", WSAGetLastError());
						(*handler)(msg);
					}
				}
				else
				{
					FD_SET(client_socket, &read_fds);
					FD_SET(client_socket, &write_fds);
					client_sockets.push_back(client_socket);
					if ((client_sockets.size() + 1) > 64)
					{
						// For the background on this check, see
						// www.tangentsoft.net/wskfaq/advanced.html#64sockets
						// The +1 is to account for the listener socket.
						//More than 63 client connections accepted. This will not work reliably on some Winsock stacks!
					}
				}
			}
#pragma endregion
#pragma region IO operations on client sockets 
			for (it = client_sockets.begin(); it != client_sockets.end();)
			{
				is_ok = true;
				if (FD_ISSET(*it, &read_fds))
				{
					is_ok = ReceiveStringData(*it, recv_buffer, ARRAYCOUNT(recv_buffer));
#ifdef  _DEBUG
					//echo backe the message that came in
					printf("recv_size: %d\n", recv_size);
					printf("received from client: %s\n", recv_buffer);
					SendStringData(*it, recv_buffer);
					printf("sent to client: %s\n", recv_buffer);
#endif _DEBUG
				}
				if (FD_ISSET(*it, &write_fds))
				{
					//is_ok = SendStringData(*it, recv_buffer);
				}

				if (!is_ok) {
					// Something bad happened on the socket, or the
					// client closed its half of the connection.  Shut
					// the conn down and remove it from the list.
					ShutdownConnection(*it);
					FD_CLR(*it, &read_fds);
					FD_CLR(*it, &write_fds);
					it = client_sockets.erase(it);
				}
				else 
				{
					// Go on to next connection
					++it;
				}

			}
#pragma endregion
		}
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "select() call failed. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	else
	{
		return false;
	}
}
bool ServerSocket::ShutdownConnection(SOCKET s)
{
	// Disallow any further data sends.  This will tell the other side
	// that we want to go away now.  If we skip this step, we don't
	// shut the connection down nicely.
	if (shutdown(s, SD_SEND) < 0)
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
		recv_size = ::recv(s, recv_buffer, ARRAYCOUNT(recv_buffer), 0);
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
	if (closesocket(s) == SOCKET_ERROR) {
		if (handler != NULL)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}

	return true;
}
bool ServerSocket::Spinning()
{
	while (true) 
	{
		//waiting for connections
		AcceptConnections();
		//Acceptor restarting...
	}
}

