#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class NetworkUtilities
	{
	public:
		LIB static bool Ping(const char* host/*host can be a domain or ipv4 address*/, unsigned int* round_trip_time = NULL/*in millisecond format*/, unsigned int* time_to_live = NULL);//round_trip_time and time_to_live are only meaning when this function returns true
		LIB static bool CheckHTTPConnection(wchar_t* url);//e.g. argument should be passed like "www.taobao.com"
		LIB static bool GetTcpFreePort(int* port);
		LIB static bool IsIP_InNetworkSegment(const char* subnet_mask, const char* ip);
		LIB static void GetIpListFromActiveAdapters(std::vector<std::string> &ip_list);
	};
}

