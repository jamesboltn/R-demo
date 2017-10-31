#pragma once
#include"NetworkInterface.h"
#include"Utilities.h"
#include"NetworkGlobalObject.h"

bool GuruLib::NetworkInterface::DnsFlushResolverCache()
{
	HINSTANCE h_dnsapi_dll = LoadLibrary(L"dnsapi.dll");
	typedef int (CALLBACK* DnsFlushProc)();
	DnsFlushProc _DnsFlushProc = (DnsFlushProc)GetProcAddress(h_dnsapi_dll, "DnsFlushResolverCache");

	if (h_dnsapi_dll == NULL)
	{
		return false;
	}
	if (_DnsFlushProc == NULL)
	{
		return false;
	}
	if ((_DnsFlushProc)() == ERROR_SUCCESS)
	{
		return true;
	}
	FreeLibrary(h_dnsapi_dll);
}

bool GuruLib::NetworkInterface::DhcpNotifyConfigChange(char* adapter_name)
{
	HINSTANCE h_dhcpcsvc_dll = LoadLibrary(L"dhcpcsvc.dll");
	if (h_dhcpcsvc_dll == NULL)
	{
		return false;
	}
	typedef int (CALLBACK* DhcpNotifyProc)(LPWSTR, LPWSTR, BOOL, DWORD, DWORD, DWORD, int);
	DhcpNotifyProc	_DhcpNotifyProc = (DhcpNotifyProc)GetProcAddress(h_dhcpcsvc_dll, "DhcpNotifyConfigChange");
	if (_DhcpNotifyProc == NULL)
	{
		return false;
	}
	
	wchar_t w_adapter_name[256];
	TypeConverter::CharStrToWCharStr(adapter_name, w_adapter_name, ArraySize(w_adapter_name));

	if ((_DhcpNotifyProc)(NULL, w_adapter_name, FALSE, 0, NULL, NULL, 0) != ERROR_SUCCESS)
	{
		return false;
	}
	FreeLibrary(h_dhcpcsvc_dll);
	return true;
}

void GuruLib::NetworkInterface::SetNameServer(const char* adapter_description, const wchar_t* preferred_name_server, const wchar_t* alternate_name_server)
{
	IP_ADAPTER_INFO* ip_adapter_info;
	if ((ip_adapter_info = GetAdaptersInfo()) != NULL)
	{
		IP_PER_ADAPTER_INFO* ip_per_adapter_info;
		do
		{
			ip_per_adapter_info = GetPerAdapterInfo(ip_adapter_info);
			if (ip_per_adapter_info != NULL)
			{
				if (adapter_description == NULL || strcmp(ip_adapter_info->Description, adapter_description) == 0)
				{
					Registry reg(HKEY_LOCAL_MACHINE);

					wchar_t w_adapter_name[256];
					TypeConverter::CharStrToWCharStr(ip_adapter_info->AdapterName, w_adapter_name, ArraySize(w_adapter_name));
					std::wstring key = L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\" + std::wstring(w_adapter_name);
					reg.SetKey(key.c_str());

					std::wstring name_server_list;
					if (preferred_name_server != NULL && wcscmp(preferred_name_server, L"") != 0)
					{
						name_server_list += preferred_name_server;
					}
					if (alternate_name_server != NULL && wcscmp(alternate_name_server, L"") != 0)
					{
						name_server_list +=  std::wstring(L",") + alternate_name_server;
					}
					reg.WriteString(L"NameServer", name_server_list.c_str());
					DhcpNotifyConfigChange(ip_adapter_info->AdapterName);
				}
			}
			ip_adapter_info = ip_adapter_info->Next;
		} while (ip_adapter_info);
		DnsFlushResolverCache();
	}
}

void GuruLib::NetworkInterface::EnableAutomaticMetric(wchar_t* adapter_description)
{
	ModifyMetric(adapter_description, ENABLE_DISABLE_AUTOMATIC_METRIC, 1);
}

void GuruLib::NetworkInterface::DisableAutomaticMetric(wchar_t* adapter_description, unsigned long metric)
{
	ModifyMetric(adapter_description, ENABLE_DISABLE_AUTOMATIC_METRIC, 0, metric);
}

void GuruLib::NetworkInterface::SetMetric(wchar_t* adapter_description, unsigned long metric)
{
	ModifyMetric(adapter_description, MODIFY_METRIC_VALUE, metric);
}

void GuruLib::NetworkInterface::ModifyMetric(wchar_t* adapter_description, MetricModifyType metric_modify_type, unsigned long value, unsigned long reserverd)
{
	IP_ADAPTER_ADDRESSES* ip_adapter_addresses;
	if ((ip_adapter_addresses = GetAdaptersAddresses()) != NULL)
	{
		MIB_IPINTERFACE_ROW mib_ipinterface_row;
		mib_ipinterface_row.Family = AF_INET;
		do
		{
			if (wcscmp(ip_adapter_addresses->FriendlyName, adapter_description) == 0)
			{
				mib_ipinterface_row.InterfaceLuid = ip_adapter_addresses->Luid;
				if (::GetIpInterfaceEntry(&mib_ipinterface_row) == NO_ERROR)
				{
					mib_ipinterface_row.SitePrefixLength = 0;
					if (metric_modify_type == ENABLE_DISABLE_AUTOMATIC_METRIC)
					{
						if (value > 0)
						{
							mib_ipinterface_row.UseAutomaticMetric = TRUE;
						}
						else
						{
							mib_ipinterface_row.Metric = reserverd;
							mib_ipinterface_row.UseAutomaticMetric = FALSE;
						}
						::SetIpInterfaceEntry(&mib_ipinterface_row);
					}
					else if(metric_modify_type == MODIFY_METRIC_VALUE)
					{
						mib_ipinterface_row.Metric = value;
						::SetIpInterfaceEntry(&mib_ipinterface_row);
					}
				}
				break;
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
	}
}

IF_OPER_STATUS GuruLib::NetworkInterface::GetInterfaceOperationalStatus(wchar_t* adapter_description)
{
	IP_ADAPTER_ADDRESSES* ip_adapter_addresses;
	if ((ip_adapter_addresses = GetAdaptersAddresses()) != NULL)
	{
		do
		{
			if (wcscmp(ip_adapter_addresses->Description, adapter_description) == 0)
			{
				return ip_adapter_addresses->OperStatus;
				break;
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
	}
}

unsigned long GuruLib::NetworkInterface::GetInterfaceIndex(wchar_t* adapter_description)
{
	IP_ADAPTER_ADDRESSES* ip_adapter_addresses;
	if ((ip_adapter_addresses = GetAdaptersAddresses()) != NULL)
	{
		do
		{
			if (wcscmp(ip_adapter_addresses->Description, adapter_description) == 0)
			{
				return ip_adapter_addresses->IfIndex;
				break;
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
	}
}

bool GuruLib::NetworkInterface::EnumInterfacesIPAddresses(std::vector<std::string> &interface_ip_adresses)
{
#ifdef STATIC_LINK
	WSADATA wsa;
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
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

	char ac[80];
	if (::gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
#ifdef STATIC_LINK
		::WSACleanup();
#endif // STATIC_LINK
		return false;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == NULL) 
	{
#ifdef STATIC_LINK
		::WSACleanup();
#endif // STATIC_LINK
		return false;
	}

	struct in_addr addr;
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		interface_ip_adresses.emplace_back(inet_ntoa(addr));
	}

#ifdef STATIC_LINK
	::WSACleanup();
#endif // STATIC_LINK

	return true;
}

void GuruLib::NetworkInterface::AddStaticIpSubnet(wchar_t* adapter_description, char* static_ip, char* subnet_mask)
{
	IP_ADAPTER_ADDRESSES* ip_adapter_addresses;
	if ((ip_adapter_addresses = GetAdaptersAddresses()) != NULL)
	{
		do
		{
			if (wcscmp(ip_adapter_addresses->Description, adapter_description) == 0)
			{
				/* IPv4 address and subnet mask we will be adding */
				UINT ip_addr = ::inet_addr(static_ip);
				UINT ip_mask = ::inet_addr(subnet_mask);
				unsigned long nte_context;
				unsigned long nte_instance;
				auto ret = ::AddIPAddress(ip_addr, ip_mask, ip_adapter_addresses->IfIndex, &nte_context, &nte_instance);
				if (ret == NO_ERROR) 
				{
					//printf("\tIP address added.\n");
				}
				else 
				{
					//printf("Error adding IP address.\n");
				}

				break;
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
	}
}

MIB_IPFORWARDTABLE* GuruLib::NetworkInterface::GetIPRoutingTable()
{
	auto ret = ::GetIpForwardTable(ip_forward_table, &size_ip_forward_table, 0);
	if (ret == ERROR_INSUFFICIENT_BUFFER) 
	{
		if (ip_forward_table != NULL)
		{
			delete ip_forward_table;
			ip_forward_table = NULL;
		}
		ip_forward_table = (MIB_IPFORWARDTABLE*)new char[size_ip_forward_table];

		if (::GetIpForwardTable(ip_forward_table, &size_ip_forward_table, 0) == NO_ERROR)
		{
			return ip_forward_table;
		}
		else
		{
			return NULL;
		}
	}
	else if (ret == NO_ERROR)
	{
		return ip_forward_table;
	}
	else
	{
		return NULL;
	}
}

MIB_IPADDRTABLE* GuruLib::NetworkInterface::GetInterfaceIPTable()
{
	auto ret = ::GetIpAddrTable(ip_addr_table, &size_ip_addr_table, 0);
	if (ret == ERROR_INSUFFICIENT_BUFFER)
	{
		if (ip_addr_table != NULL)
		{
			delete ip_addr_table;
			ip_addr_table = NULL;
		}
		ip_addr_table = (MIB_IPADDRTABLE*)new char[size_ip_addr_table];

		if (::GetIpAddrTable(ip_addr_table, &size_ip_addr_table, 0) == NO_ERROR)
		{
			return ip_addr_table;
		}
		else
		{
			return NULL;
		}
	}
	else if (ret == NO_ERROR)
	{
		return ip_addr_table;
	}
	else
	{
		return NULL;
	}
}

GuruLib::NetworkInterface::~NetworkInterface()
{
	if (ip_forward_table != NULL)
	{
		delete ip_forward_table;
		ip_forward_table = NULL;
	}
	if (ip_addr_table != NULL)
	{
		delete ip_addr_table;
		ip_addr_table = NULL;
	}
}
