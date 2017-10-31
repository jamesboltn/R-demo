#pragma once
#define DLL_EXPORT
#include"SocketBaseTCP.h"

namespace GuruLib
{
	class TCPClient :
		public SocketBaseTCP
	{
	private:
		bool ShutdownConnection();//gracefully shutdown the connection
	public:
		bool Connect(const char* ip_addr, unsigned short port);
		bool GetIPByDomain(const char* domain, char* ip_addr, unsigned int size);
		bool SendString(const char* data, int* bytes_totally_sent);//return value means if there is any error happeds during the send aciton;to get how many bytes are actually sent, see the second parameter  
		bool ReceiveString(char* buffer, int length);
	};
}
