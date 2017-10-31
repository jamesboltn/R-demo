#pragma once

namespace GuruLib
{
	typedef enum
	{
		IPv4,
		IPv6
	}InternetProtocolVersion;

	typedef void(*SocketErrorMessageHandler)(const char* msg);
}

