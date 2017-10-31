#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"NetworkAdapterHelper.h"
#include"Registry.h"

namespace GuruLib
{
	class NetworkInterface :
		public NetworkAdapterHelper
	{
	private:
		typedef enum
		{
			ENABLE_DISABLE_AUTOMATIC_METRIC,
			MODIFY_METRIC_VALUE
		} MetricModifyType;

		unsigned long size_ip_forward_table = 0;
		MIB_IPFORWARDTABLE* ip_forward_table = NULL;
		unsigned long size_ip_addr_table = 0;
		MIB_IPADDRTABLE* ip_addr_table = NULL;

		void ModifyMetric(wchar_t* adapter_description, MetricModifyType metric_modify_type, unsigned long value, unsigned long reserverd = -1);//if metric_modify_type is ENABLE_DISABLE_AUTOMATIC_METRIC, then value is enable flag, else value is metric. If value is flag and is greater than zero, then means enable, else disable. If value is disable, then reserverd is metric
	protected:

	public:
		LIB ~NetworkInterface();

		LIB static bool DnsFlushResolverCache();
		LIB static bool DhcpNotifyConfigChange(char* adapter_name);

		LIB void SetNameServer(const char* adapter_description, const wchar_t* preferred_name_server, const wchar_t* alternate_name_server);//this function set preferred name server and alternate server of all adapters
		LIB void EnableAutomaticMetric(wchar_t* adapter_description);
		LIB void DisableAutomaticMetric(wchar_t* adapter_description, unsigned long metric);
		LIB void SetMetric(wchar_t* adapter_description, unsigned long metric);
		LIB IF_OPER_STATUS GetInterfaceOperationalStatus(wchar_t* adapter_description);
		LIB unsigned long GetInterfaceIndex(wchar_t* adapter_description);

		//This is not a much useful function as you see. It only enumerates all the ip addresses of all the interfaces while not map these ip addresses to their corresponding interfaces 
		LIB bool EnumInterfacesIPAddresses(std::vector<std::string> &interface_ip_adresses);

		LIB void AddStaticIpSubnet(wchar_t* adapter_description, char* static_ip, char* subnet_mask);
		LIB MIB_IPFORWARDTABLE* GetIPRoutingTable();
		LIB MIB_IPADDRTABLE* GetInterfaceIPTable();
	};

	//DNSOption::SetNameServer()
	//1.if SetNameServer is NULL, then  this function set domain name server of all adapters
	//2.if alternate_name_server is not NULL, then preferred_name_server must not be NULL
	//3.if preferred_name_server is NULL, then alternate_name_server must be NULL
}
