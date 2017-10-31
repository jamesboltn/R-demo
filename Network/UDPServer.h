#pragma once
#include"ServerBase.h"

namespace GuruLib
{
	class UDPServer :
		public ServerBase<UDPServer>
	{
	private:
		int udp_send_max_msg_size;
		bool SendString(SOCKET s, const char* data, int* bytes_totally_sent);
	public:
		~UDPServer();
		UDPServer();
		void Listen(unsigned short port);//note that we need not bind to a particular ip since all incoming connections from all network interfaces are acceptable
		bool ShutdownConnection(SOCKET s);//gracefully shutdown the connection
	};
}
