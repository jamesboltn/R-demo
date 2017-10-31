#pragma once
#include"NetworkUtilities.h"
#include"NetworkGlobalObject.h"
#include"StringAssist.h"
#include"NetworkAdapterHelper.h"

bool GuruLib::NetworkUtilities::Ping(const char* host/*host can be a domain or ipv4 address*/, unsigned int* round_trip_time/*in millisecond format*/, unsigned int* time_to_live)
{
	// Structures required to use functions in ICMP.DLL
	typedef struct {
		unsigned char Ttl;                         // Time To Live
		unsigned char Tos;                         // Type Of Service
		unsigned char Flags;                       // IP header flags
		unsigned char OptionsSize;                 // Size in bytes of options data
		unsigned char *OptionsData;                // Pointer to options data
	} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;
	typedef struct {
		DWORD Address;                             // Replying address
		unsigned long  Status;                     // Reply status
		unsigned long  RoundTripTime;              // RTT in milliseconds
		unsigned short DataSize;                   // Echo data size
		unsigned short Reserved;                   // Reserved for system use
		void *Data;                                // Pointer to the echo data
		IP_OPTION_INFORMATION Options;             // Reply options
	} IP_ECHO_REPLY, *PIP_ECHO_REPLY;


#ifdef STATIC_LINK
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
	{
		return false;
	}
	else if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		::WSACleanup();
		return false;
	}
#else
	if (!NetworkGlobal::wsa_startup)
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR)
		{
			if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
			{
				::WSACleanup();
				return false;
			}
			NetworkGlobal::wsa_startup = true;
		}
		else
		{
			return false;
		}
	}
#endif // STATIC_LINK

	bool succeed = true;
	// Load the ICMP.DLL
	HINSTANCE h_icmp_dll = ::LoadLibrary(L"ICMP.DLL");
	if (h_icmp_dll == NULL)
	{
		succeed = false;
	}

	// Look up an IP address for the given host name
	struct hostent* phe;
	if ((phe = gethostbyname(host)) == NULL)
	{
		succeed = false;
	}

	// Get handles to the functions inside ICMP.DLL that we'll need
	typedef HANDLE(WINAPI* pfnHV)(VOID);
	typedef BOOL(WINAPI* pfnBH)(HANDLE);
	typedef DWORD(WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
		PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); // evil, no?
	pfnHV pIcmpCreateFile = (pfnHV)GetProcAddress(h_icmp_dll, "IcmpCreateFile");
	pfnBH pIcmpCloseHandle = (pfnBH)GetProcAddress(h_icmp_dll, "IcmpCloseHandle");
	pfnDHDPWPipPDD pIcmpSendEcho = (pfnDHDPWPipPDD)GetProcAddress(h_icmp_dll, "IcmpSendEcho");
	if ((pIcmpCreateFile == NULL) || (pIcmpCloseHandle == 0) || (pIcmpSendEcho == NULL))
	{
		succeed = false;
	}

	// Open the ping service
	HANDLE hIP = pIcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE)
	{
		succeed = false;
		//std::cerr << "Unable to open ping service." << std::endl;
	}

	// Build ping packet
	char acPingBuffer[64];
	memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
	PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
	if (pIpe == NULL)
	{
		succeed = false;
	}
	pIpe->Data = acPingBuffer;
	pIpe->DataSize = sizeof(acPingBuffer);

	// Send the ping packet
	DWORD dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
	if (dwStatus != 0)
	{
		//std::cout << "Addr: " <<
		//int(LOBYTE(LOWORD(pIpe->Address))) << "." <<
		//int(HIBYTE(LOWORD(pIpe->Address))) << "." <<
		//int(LOBYTE(HIWORD(pIpe->Address))) << "." <<
		//int(HIBYTE(HIWORD(pIpe->Address))) << ", " <<
		//"RTT: " << int(pIpe->RoundTripTime) << "ms, " <<
		//"TTL: " << int(pIpe->Options.Ttl) << std::endl;
		if (round_trip_time != NULL)
		{
			*round_trip_time = unsigned int(pIpe->RoundTripTime);
		}
		if (time_to_live != NULL)
		{
			*time_to_live = unsigned int(pIpe->Options.Ttl);
		}
	}
	else
	{
		succeed = false;
		//std::cerr << "Error obtaining info from ping packet." << std::endl;
	}

	// Shut down...
	::GlobalFree(pIpe);
	::FreeLibrary(h_icmp_dll);

#ifdef STATIC_LINK
	::WSACleanup();
#endif // STATIC_LINK

	return succeed;
}

bool GuruLib::NetworkUtilities::CheckHTTPConnection(wchar_t* url)
{
	return InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, 0);
}

bool GuruLib::NetworkUtilities::GetTcpFreePort(int* port)
{
#ifdef STATIC_LINK
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
		{
			return false;
		}
		else if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
		{
			::WSACleanup();
			return false;
		}
#else
	if (!NetworkGlobal::wsa_startup)
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR)
		{
			if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
			{
				::WSACleanup();
				return false;
			}
			NetworkGlobal::wsa_startup = true;
		}
		else
		{
			return false;
		}
	}
#endif // STATIC_LINK

	struct sockaddr_in serv_addr;
	::memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
	serv_addr.sin_port = ::htons(0);
	SOCKET serv_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock == INVALID_SOCKET)
	{
		//printf("socket() error:%s\n", strerror(errno));
#ifdef STATIC_LINK
		WSACleanup();
#endif // STATIC_LINK
		return false;
	}
	if (::bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		//printf("bind() error:%s\n", strerror(errno));
		::closesocket(serv_sock);
#ifdef STATIC_LINK
		WSACleanup();
#endif // STATIC_LINK
		return false;
	}
	int len = sizeof(serv_addr);
	if (::getsockname(serv_sock, (struct sockaddr*)&serv_addr, &len) == SOCKET_ERROR)
	{
		//printf("getsockname() error:%s\n", strerror(errno));
		::closesocket(serv_sock);
#ifdef STATIC_LINK
		WSACleanup();
#endif // STATIC_LINK
		return false;
	}
	if (::closesocket(serv_sock) != 0)
	{
		//printf("close() error:%s\n", strerror(errno));
#ifdef STATIC_LINK
		WSACleanup();
#endif // STATIC_LINK
		return false;
	}
	
	*port = serv_addr.sin_port;
#ifdef STATIC_LINK
	WSACleanup();
#endif // STATIC_LINK
	return true;
}

bool GuruLib::NetworkUtilities::IsIP_InNetworkSegment(const char* subnet_mask, const char* ip)
{
	std::string _ip = ip;
	std::vector<std::string> ip_tokens;

	StdString::Split(_ip, '.', ip_tokens);
	if (ip_tokens.size() != 4)
	{
		return false;
	}

	unsigned long numeric_ip = pow(2,24) * atoi(ip_tokens[0].c_str()) + pow(2,16)* atoi(ip_tokens[1].c_str()) + pow(2,8)* atoi(ip_tokens[2].c_str()) + atoi(ip_tokens[3].c_str());

	std::string _subnet_mask = subnet_mask;
	std::vector<std::string> tokens;
	StdString::Split(_subnet_mask, '/', tokens);
	if (tokens.size() != 2)
	{
		return false;
	}

	int network_bit_len = atoi(tokens[1].c_str());
	int right_shift_offset = 32 - network_bit_len;

	std::vector<std::string> subnet_mask_tokens;
	StdString::Split(tokens[0], '.', subnet_mask_tokens);
	if (subnet_mask_tokens.size() != 4)
	{
		return false;
	}

	unsigned long subnet_mask_ip = pow(2, 24) * atoi(subnet_mask_tokens[0].c_str()) + pow(2, 16)* atoi(subnet_mask_tokens[1].c_str()) + pow(2, 8)* atoi(subnet_mask_tokens[2].c_str()) + atoi(subnet_mask_tokens[3].c_str());

	return (numeric_ip >> right_shift_offset) == (subnet_mask_ip >> right_shift_offset);
}

void GuruLib::NetworkUtilities::GetIpListFromActiveAdapters(std::vector<std::string> &ip_list)
{
	ip_list.clear();

	NetworkAdapterHelper network_adapter_helper;
	IP_ADAPTER_INFO* ip_adapter_info;
	std::unordered_map<DWORD, std::string> adapterindex_ip_map;
	if ((ip_adapter_info = network_adapter_helper.GetAdaptersInfo()) != NULL)
	{
		do
		{
			if (strcmp(ip_adapter_info->IpAddressList.IpAddress.String, "0.0.0.0") == 0)
			{
				ip_adapter_info = ip_adapter_info->Next;
				continue;
			}

			adapterindex_ip_map[ip_adapter_info->Index] = ip_adapter_info->IpAddressList.IpAddress.String;

			ip_adapter_info = ip_adapter_info->Next;
		} while (ip_adapter_info);
	}

	if (adapterindex_ip_map.empty())
	{
		return;
	}

	IP_ADAPTER_ADDRESSES* ip_adapter_addresses;
	if ((ip_adapter_addresses = network_adapter_helper.GetAdaptersAddresses()) != NULL)
	{
		do
		{
			if (ip_adapter_addresses->OperStatus == IfOperStatusUp)
			{
				if (adapterindex_ip_map.find(ip_adapter_addresses->IfIndex) != adapterindex_ip_map.end())
				{
					ip_list.push_back(adapterindex_ip_map[ip_adapter_addresses->IfIndex]);
				}
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
	}
}