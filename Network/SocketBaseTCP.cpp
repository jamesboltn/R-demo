#pragma once
#include"SocketBaseTCP.h"
#include"NetworkGlobalObject.h"

bool GuruLib::SocketBaseTCP::ForceShutdownConnection()
{
	//call shutdown() with the how parameter set to SD_BOTH(¡°both directions¡±), optionally followed by a closesocket() call
	//only forcibly shut down the connection when your program or the remote peer is 
	if (shutdown(master_socket, SD_BOTH) < 0)
	{
		if (handler != NULL)
		{
			sprintf_s(msg, ArraySize(msg), "failed to shutdown connection. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}

	if (closesocket(master_socket) == SOCKET_ERROR) {
		if (handler != NULL)
		{
			sprintf_s(msg, ArraySize(msg), "failed to close socket. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	return true;
}

GuruLib::SocketBaseTCP::SocketBaseTCP()
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