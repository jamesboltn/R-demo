#pragma once
#include"SocketBase.h"
#include"NetworkGlobalObject.h"

//TODO:merge ICMP Ping code from WinSock FAQ

GuruLib::SocketBase::SocketBase(InternetProtocolVersion ip_ver, SocketErrorMessageHandler _handler) :
	handler(_handler)
{
#ifdef STATIC_LINK
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to init socket. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
		}
		else
		{
			if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
			{
				::WSACleanup();
			}
			else
			{
				wsa_startup = true;
			}
		}
#else
	if (!NetworkGlobal::wsa_startup)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
		{
			if (handler != NULL)
			{
				sprintf_s(msg, ArraySize(msg), "failed to init socket. error code : %d", WSAGetLastError());
				(*handler)(msg);
			}
		}
		else
		{
			if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
			{
				::WSACleanup();
			}
			else
			{
				NetworkGlobal::wsa_startup = true;
			}
		}
	}
#endif // STATIC_LINK
}

GuruLib::SocketBase::~SocketBase()
{
#ifdef STATIC_LINK
	if (wsa_startup)
	{
		::WSACleanup();
	}
#endif // STATIC_LINK
}

bool GuruLib::SocketBase::UnitSendForText(SOCKET s, const char* data, int* bytes_totally_sent, int text_len)
{
	*bytes_totally_sent = 0;

	int bytes_per_send = ::send(s, data, text_len, 0);

	while (*bytes_totally_sent < text_len && bytes_per_send != SOCKET_ERROR)
	{
		*bytes_totally_sent += bytes_per_send;
		bytes_per_send = ::send(s, &data[*bytes_totally_sent], text_len - *bytes_totally_sent, 0);
	}

	if (bytes_per_send == SOCKET_ERROR)
	{
		if (handler != NULL)
		{
			sprintf_s(msg, ArraySize(msg), "failed to send string data to peer. error code : %d", WSAGetLastError());
			(*handler)(msg);
		}
		return false;
	}
	return true;
}
