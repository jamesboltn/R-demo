#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	//NOTE:this class is not thread safe;
	class NetworkInterfaceHelper
	{
	private:
		unsigned long size_ip_interface_info = 0;
		IP_INTERFACE_INFO* ip_interface_info = NULL;
		IP_INTERFACE_INFO* GetInterfacesInfo();//if succeed, obtains the list of the network interfaces with IPv4 enabled on the local system.,else return NULL
	public:
		LIB ~NetworkInterfaceHelper();
	};
}

