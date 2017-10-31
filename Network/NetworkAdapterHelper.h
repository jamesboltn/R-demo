#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	//NOTE:this class is not thread safe;
	class NetworkAdapterHelper
	{
	private:
		unsigned long size_ip_adapter_info = 0;
		IP_ADAPTER_INFO* ip_adapter_info = NULL;
		unsigned long size_ip_per_adapter_info = 0;
		IP_PER_ADAPTER_INFO* ip_per_adapter_info = NULL;
		unsigned long size_ip_adapter_addresses = 0;
		IP_ADAPTER_ADDRESSES* ip_adapter_addresses = NULL;
#ifndef _DEBUG
	protected:
		IP_PER_ADAPTER_INFO* GetPerAdapterInfo(IP_ADAPTER_INFO* ip_adapter_info);//if fail, return NULL
#else
	public:
		IP_PER_ADAPTER_INFO* GetPerAdapterInfo(IP_ADAPTER_INFO* ip_adapter_info);//if fail, return NULL
#endif // !_DEBUG
	public:
		LIB ~NetworkAdapterHelper();

		LIB IP_ADAPTER_ADDRESSES* GetAdaptersAddresses();//if succeed, return header item of a linked-list of IP_ADAPTER_ADDRESSES, else return NULL
		LIB bool GetMacAddressByInterfaceIndex(unsigned long if_index, std::string& mac);

		LIB IP_ADAPTER_INFO* GetAdaptersInfo();//if succeed, return header item of a linked-list of IP_ADAPTER_INFO,else return NULL
	};
}

