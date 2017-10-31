#pragma once
#include"SocketBase.h"

namespace GuruLib
{
	template<typename ChildClass>
	class ServerBase :
		public SocketBase
	{
	private:
		bool is_ok;
		std::vector<SOCKET> client_sockets;
		fd_set read_fds;//set of socket descriptors
		fd_set write_fds;
		SOCKET client_socket;
		ChildClass* server;
		bool ReceiveString(SOCKET s, char* buffer, int length)
		{
			//get details of the client
			//getpeername(*it, (struct sockaddr*)&_sockaddr_in, (int*)&sockaddr_in_size);
			//Somebody disconnected , get his details and print
			//printf("Host disconnected , ip %s , port %d \n", inet_ntoa(_sockaddr_in.sin_addr), ntohs(_sockaddr_in.sin_port));

			recv_size = ::recv(s, buffer, length, 0);
			if (recv_size == SOCKET_ERROR)
			{
				// Something bad happened on the socket.  It could just be a
				// "would block" notification, or it could be something more
				// serious.  Let caller handle the latter case.  WSAEWOULDBLOCK
				// can happen after select() says a socket is readable under
				// Win9x: it doesn't happen on WinNT/2000 or on Unix.
#ifdef _DEBUG
				sprintf_s(msg, ArraySize(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
				printf("%s\n", msg);
#endif // _DEBUG
				if (handler != NULL)
				{
					sprintf_s(msg, ArraySize(msg), "failed to receive string data from server. error code : %d", WSAGetLastError());
					(*handler)(msg);
				}
				return false;
			}
			else if (recv_size == 0 || recv_size == WSAECONNRESET)
			{
				//Somebody disconnected
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
	protected:
		bool AcceptConnections()
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
					int addrlen;
					struct sockaddr_in _sockaddr_in;
					if ((client_socket = accept(master_socket, (struct sockaddr*)&_sockaddr_in, (int*)&addrlen)) == INVALID_SOCKET)
					{
						if (handler != NULL)
						{
							sprintf_s(msg, ArraySize(msg), "accept() call failed. error code : %d", WSAGetLastError());
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
				for (auto it = client_sockets.begin(); it != client_sockets.end();)
				{
					is_ok = true;
					if (FD_ISSET(*it, &read_fds))
					{
						is_ok = ReceiveString(*it, recv_buffer, ArraySize(recv_buffer));
					}
					if (FD_ISSET(*it, &write_fds))
					{
						//is_ok = SendStringData(*it, recv_buffer);
					}

					if (!is_ok) {
						// Something bad happened on the socket, or the
						// client closed its half of the connection.  Shut
						// the conn down and remove it from the list.
						server->ShutdownConnection(*it);
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
				sprintf_s(msg, ArraySize(msg), "select() call failed. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
			return false;
		}
	public:
		explicit ServerBase(ChildClass* _server)
		{
			server = _server;
		}
	};
}
