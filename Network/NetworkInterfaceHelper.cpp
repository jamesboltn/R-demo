#pragma once
#include"NetworkInterfaceHelper.h"

IP_INTERFACE_INFO* GuruLib::NetworkInterfaceHelper::GetInterfacesInfo()
{
	auto ret = ::GetInterfaceInfo(ip_interface_info, &size_ip_interface_info);
	if (ret == ERROR_INSUFFICIENT_BUFFER) // out of buff
	{
		if (ip_interface_info != NULL)
		{
			delete ip_interface_info;
			ip_interface_info = NULL;
		}
		ip_interface_info = (IP_INTERFACE_INFO*)new char[size_ip_interface_info];

		if (::GetInterfaceInfo(ip_interface_info, &size_ip_interface_info) == NO_ERROR)
		{
			return ip_interface_info;
		}
		else
		{
			return NULL;
		}
	}
	else if(ret == NO_ERROR)
	{
		return ip_interface_info;
	}
	else
	{
		return NULL;
	}
}

GuruLib::NetworkInterfaceHelper::~NetworkInterfaceHelper()
{
	if (ip_interface_info != NULL)
	{
		delete ip_interface_info;
		ip_interface_info = NULL;
	}
}
