#pragma once
#include"ServerBase.h"

namespace GuruLib
{
	class TCPServer :
		public ServerBase<TCPServer>
	{
	private:
		bool SendString(SOCKET s, const char* data, int* bytes_totally_sent);
	public:
		~TCPServer();
		TCPServer();
		void Listen(unsigned short port);//note that we need not bind to a particular ip since all incoming connections from all network interfaces are acceptable
		bool ShutdownConnection(SOCKET s);//gracefully shutdown the connection
	};
}
