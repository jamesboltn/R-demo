#pragma once
#include"InternetOption.h"
#include"Registry.h"

GuruLib::InternetOption::InternetOption()
{
	list.dwOptionError = 0;
	list.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
}

void GuruLib::InternetOption::ReadCheckStatusFromRegistry(BOOL* auto_detect, BOOL* use_script)
{
	Registry reg(HKEY_CURRENT_USER);
	reg.SetKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections", FALSE);
	BYTE buffer[400];
	//the ninth byte of value of name DefaultConnectionSettings stores check statuses of 'auto detect' and 'use script' 
	reg.ReadByteArray(L"DefaultConnectionSettings", buffer, ArraySize(buffer));

	switch (buffer[8])
	{
	case 0x01://neither of them are checked
		*auto_detect = FALSE;
		*use_script = FALSE;
		break;
	case 0x05://just the use script is checked
		*auto_detect = FALSE;
		*use_script = TRUE;
		break;
	case 0x09://just the auto detect is checked
		*auto_detect = TRUE;
		*use_script = FALSE;
		break;
	case 0x0d://both of them are checked
		*auto_detect = TRUE;
		*use_script = TRUE;
		break;
	default://if ninth byte does not match any of the above cases, set both of the two checkboxes to unchecked
		*auto_detect = FALSE;
		*use_script = FALSE;
		break;
	}
}

void GuruLib::InternetOption::ReadAutoConfigScriptUrl(std::wstring &script_url)
{
	INTERNET_PER_CONN_OPTION         Option[1];
	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;

	list.pszConnection = NULL;
	list.dwOptionCount = 1;
	list.pOptions = Option;

	if (!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, &size_of_list))
	{
		return;
	}

	if (Option[0].Value.pszValue != NULL)
	{
		script_url = Option[0].Value.pszValue;
		GlobalFree(Option[0].Value.pszValue);
	}
}

bool GuruLib::InternetOption::WriteAutoConfigScriptUrl(std::wstring &script_url)
{
	INTERNET_PER_CONN_OPTION         Option[1];

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[0].Value.pszValue = const_cast<wchar_t*>(script_url.c_str());

	list.pszConnection = NULL;
	list.dwOptionCount = 1;
	list.pOptions = Option;

	if (!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, size_of_list))
	{
		return false;
	}
	else
	{
		InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
		return true;
	}
}

bool GuruLib::InternetOption::WriteAutoConfigScriptUrl(const wchar_t* connection_name, wchar_t* script_url)
{
	INTERNET_PER_CONN_OPTION         Option[1];

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[0].Value.pszValue = script_url;

	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = (connection_name == NULL) ? NULL : const_cast<wchar_t*>(connection_name);
	list.dwOptionCount = 1;
	list.pOptions = Option;

	if (!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, size_of_list))
	{
		return false;
	}
	else
	{
		InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
		return true;
	}
}

bool GuruLib::InternetOption::WriteProxyServerAddressPort(const wchar_t* connection_name, wchar_t* proxy_address_port)
{
	INTERNET_PER_CONN_OPTION options[2];
	BOOL    bReturn;

	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = (connection_name == NULL) ? NULL : const_cast<wchar_t*>(connection_name);

	// Set three options.
	list.dwOptionCount = 2;
	list.pOptions = options;

	// Set proxy name.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list.pOptions[0].Value.pszValue = proxy_address_port;

	// Set proxy override.
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[1].Value.pszValue = TEXT("local");

	// Set the options on the connection.
	bReturn = InternetSetOption(NULL,
		INTERNET_OPTION_PER_CONNECTION_OPTION, &list, size_of_list);

	if (bReturn)
	{
		InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	}
	return bReturn;
}

void GuruLib::InternetOption::EnumConnectionNames(std::vector<std::wstring> OUT &connection_names)
{
	unsigned long lpcb = sizeof(RASENTRYNAME);
	unsigned long array_size = 0;
	RASENTRYNAME* arr_ras_entry_name = new RASENTRYNAME;
	arr_ras_entry_name[0].dwSize = lpcb;
	unsigned long ret = RasEnumEntries(NULL, NULL, arr_ras_entry_name, &lpcb, &array_size);

	if ((ret != ERROR_SUCCESS) && (ERROR_BUFFER_TOO_SMALL != ret))
	{
		array_size = 0;
	}

	// 如果電話本裏有拔號連接  
	if (array_size != 0)
	{
		delete arr_ras_entry_name;
		arr_ras_entry_name = new RASENTRYNAME[array_size];
		for (int i = 0; i < array_size; ++i)
		{
			arr_ras_entry_name[i].dwSize = sizeof(RASENTRYNAME);
		}
		if (RasEnumEntries(NULL, NULL, arr_ras_entry_name, &lpcb, &array_size) != 0)
		{
			delete[] arr_ras_entry_name;
			arr_ras_entry_name = NULL;
			return;
		}
		// 將所有的拔號連接的名稱追加到 存儲連接名稱的數組 剛才已經存了一個DefaultLAN了。 
		for (int i = 0; i < array_size; ++i)
		{
			connection_names.emplace_back(arr_ras_entry_name[i].szEntryName);
		}
		delete[] arr_ras_entry_name;
		arr_ras_entry_name = NULL;
	}
	else
	{
		delete arr_ras_entry_name;
		arr_ras_entry_name = NULL;
	}
}

void GuruLib::InternetOption::UseProxyServer(const wchar_t* connection_name, bool use)
{
	Check(connection_name, PROXY_TYPE_PROXY, use);
}

void GuruLib::InternetOption::UseAutoDetectSettings(const wchar_t* connection_name, bool use)
{
	Check(connection_name, PROXY_TYPE_AUTO_DETECT, use);
}

void GuruLib::InternetOption::UseAutoConfigScript(const wchar_t* connection_name, bool use)
{
	Check(connection_name, PROXY_TYPE_AUTO_PROXY_URL, use);
}

void GuruLib::InternetOption::Check(const wchar_t* connection_name, unsigned long flag, bool use)
{
	INTERNET_PER_CONN_OPTION options[1];
	BOOL    bReturn;
	
	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = (connection_name == NULL) ? NULL : const_cast<wchar_t*>(connection_name);

	// Set options.
	list.dwOptionCount = 1;
	list.pOptions = options;

	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	if (!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, &size_of_list))
	{
		return;
	}

	// Set flags.
	if (use)
	{
		list.pOptions[0].Value.dwValue |= flag;
	}
	else
	{
		list.pOptions[0].Value.dwValue &= ~flag;
	}

	// Set the options on the connection.
	bReturn = InternetSetOption(NULL,
		INTERNET_OPTION_PER_CONNECTION_OPTION, &list, size_of_list);

	if (bReturn)
	{
		InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	}
}


