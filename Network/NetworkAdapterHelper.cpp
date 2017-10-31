#pragma once
#include"NetworkAdapterHelper.h"

IP_ADAPTER_INFO* GuruLib::NetworkAdapterHelper::GetAdaptersInfo()
{
	auto ret = ::GetAdaptersInfo(ip_adapter_info, &size_ip_adapter_info);
	if (ret == ERROR_BUFFER_OVERFLOW) // out of buff
	{
		if(ip_adapter_info != NULL)
		{
			delete ip_adapter_info;
			ip_adapter_info = NULL;
		}
		ip_adapter_info = (IP_ADAPTER_INFO*)new char[size_ip_adapter_info];

		if (::GetAdaptersInfo(ip_adapter_info, &size_ip_adapter_info) != ERROR_SUCCESS)
		{
			return NULL;
		}
		else
		{
			return ip_adapter_info;
		}
	}
	else if(ret == ERROR_SUCCESS)
	{
		return ip_adapter_info;
	}
	else
	{
		return NULL;
	}
}

GuruLib::NetworkAdapterHelper::~NetworkAdapterHelper()
{
	if (ip_adapter_info != NULL)
	{
		delete ip_adapter_info;
		ip_adapter_info = NULL;
	}
	if (ip_per_adapter_info != NULL)
	{
		delete ip_per_adapter_info;
		ip_per_adapter_info = NULL;
	}
	if (ip_adapter_addresses != NULL)
	{
		delete ip_adapter_addresses;
		ip_adapter_addresses = NULL;
	}
}

IP_PER_ADAPTER_INFO* GuruLib::NetworkAdapterHelper::GetPerAdapterInfo(IP_ADAPTER_INFO* ip_adapter_info)
{
	auto ret = ::GetPerAdapterInfo(ip_adapter_info->Index, ip_per_adapter_info, &size_ip_per_adapter_info);
	if (ret == ERROR_BUFFER_OVERFLOW) // out of buff
	{
		if (ip_per_adapter_info != NULL)
		{
			delete ip_per_adapter_info;
			ip_per_adapter_info = NULL;
		}
		ip_per_adapter_info = (IP_PER_ADAPTER_INFO*)new char[size_ip_per_adapter_info];
		if (::GetPerAdapterInfo(ip_adapter_info->Index, ip_per_adapter_info, &size_ip_per_adapter_info) == ERROR_SUCCESS)
		{
			return ip_per_adapter_info;
		}
		else
		{
			return NULL;
		}
	}
	else if(ret == ERROR_SUCCESS)
	{
		return ip_per_adapter_info;
	}
	else
	{
		return NULL;
	}
}

IP_ADAPTER_ADDRESSES* GuruLib::NetworkAdapterHelper::GetAdaptersAddresses()
{
	auto ret = ::GetAdaptersAddresses(AF_INET, 0, NULL, ip_adapter_addresses, &size_ip_adapter_addresses);
	if (ret == ERROR_BUFFER_OVERFLOW) // out of buff
	{
		if (ip_adapter_addresses != NULL)
		{
			delete ip_adapter_addresses;
			ip_adapter_addresses = NULL;
		}
		ip_adapter_addresses = (IP_ADAPTER_ADDRESSES*)new char[size_ip_adapter_addresses];

		if (::GetAdaptersAddresses(AF_INET, 0, NULL, ip_adapter_addresses, &size_ip_adapter_addresses) != ERROR_SUCCESS)
		{
			return NULL;
		}
		else
		{
			return ip_adapter_addresses;
		}
	}
	else if (ret == ERROR_SUCCESS)
	{
		return ip_adapter_addresses;
	}
	else
	{
		return NULL;
	}
}

bool GuruLib::NetworkAdapterHelper::GetMacAddressByInterfaceIndex(unsigned long if_index, std::string& mac)
{
	IP_ADAPTER_ADDRESSES* ip_adapter_addresses = GetAdaptersAddresses();

	if (ip_adapter_addresses != NULL)
	{
		do
		{
			if (ip_adapter_addresses->IfIndex == if_index)
			{
				char hex_part[5];
				for (int i = 0; i < ip_adapter_addresses->PhysicalAddressLength; ++i)
				{
					if (i == 0)
					{
						sprintf_s(hex_part, ArraySize(hex_part), "%02x", ip_adapter_addresses->PhysicalAddress[i]);
						mac = hex_part;
					}
					else
					{
						sprintf_s(hex_part, ArraySize(hex_part), "%02x", ip_adapter_addresses->PhysicalAddress[i]);
						mac += std::string(":") + hex_part;
					}
				}
				return true;
			}
			ip_adapter_addresses = ip_adapter_addresses->Next;
		} while (ip_adapter_addresses);
		return false;
	}
	else
	{
		return false;
	}
}