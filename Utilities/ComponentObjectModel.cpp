#pragma once
#include"ComponentObjectModel.h"
#include"PreInclude.h"

GuruLib::ComponentObjectModel::ComponentObjectModel()
{
	HRESULT hr;
	hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
	if (!FAILED(hr))
	{
		hr = ::CoInitializeSecurity(
			NULL,                        // Security descriptor    
			-1,                          // COM negotiates authentication service
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities of the client or server
			NULL);                       // Reserved

		if (FAILED(hr))
		{
			::CoUninitialize();
		}
		else
		{
			com_initialized = true;
		}
	}
}

GuruLib::ComponentObjectModel::~ComponentObjectModel()
{
	if (com_initialized)
	{
		::CoUninitialize();
	}
}