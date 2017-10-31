#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	//NOTE:this class is not multithread safe. Take care 
	class InternetOption
	{
	private:
		INTERNET_PER_CONN_OPTION_LIST list;
		unsigned long size_of_list = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	protected:

	public:
		LIB InternetOption();
		LIB void ReadCheckStatusFromRegistry(BOOL* auto_detect, BOOL* use_script);

		//checkbox:use auto detect settings
		LIB void UseAutoDetectSettings(const wchar_t* connection_name, bool use);

		//checkbox:use auto config script
		LIB void UseAutoConfigScript(const wchar_t* connection_name, bool use);

		//fill:auto config script url
		LIB void ReadAutoConfigScriptUrl(std::wstring &script_url);
		LIB bool WriteAutoConfigScriptUrl(std::wstring &script_url);
		LIB bool WriteAutoConfigScriptUrl(const wchar_t* connnection_name, wchar_t* script_url);

		//checkbox:use proxy server
		LIB void UseProxyServer(const wchar_t* connection_name, bool use);

		//fill:proxy server address port
		LIB bool WriteProxyServerAddressPort(const wchar_t* connection_name, wchar_t* proxy_address_port);

		LIB void Check(const wchar_t* connection_name, unsigned long flag, bool use);
		LIB void EnumConnectionNames(std::vector<std::wstring> OUT &connection_names);
	};
}
