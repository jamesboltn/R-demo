#pragma once
#define DLL_EXPORT
#include"Preinclude.h"
#include"NetworkDef.h"

namespace GuruLib
{
	class SocketBase
	{
	private:
		WSADATA wsa;
	protected:
		char msg[100];
		SocketErrorMessageHandler handler;
		bool is_socket_initialized;
#ifdef STATIC_LINK
		bool wsa_startup = false;
#endif // STATIC_LINK

		SOCKET master_socket;

		bool UnitSendForText(SOCKET s, const char* data, int* bytes_totally_sent, int text_len);//return value is actually bytes sent;text len does not count the NULL terminator;对于UDP连接，text_len最大值为网络允许的最大写入缓存（可通过getsockopt以选项SO_MAX_MSG_SIZE查得），对于TCP连接，text_len不作要求
	public:
		char recv_buffer[2000];
		int recv_size;

		SocketBase(InternetProtocolVersion ip_ver = IPv4, SocketErrorMessageHandler _handler = NULL);
		~SocketBase();
	};
}
