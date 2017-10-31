#pragma once
#include"SocketBaseUDP.h"
#include"NetworkGlobalObject.h"

GuruLib::SocketBaseUDP::SocketBaseUDP()
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
