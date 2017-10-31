#pragma once
#include"WindowsManagementInstrumentation.h"

GuruLib::WindowsManagementInstrumentation::WindowsManagementInstrumentation()
{
	if (com_initialized)
	{
		HRESULT hr;
		hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&i_wbem_locator);

		if (!FAILED(hr))
		{
			// Connect to the root\default namespace with the current user.
			hr = i_wbem_locator->ConnectServer(
				BSTR(L"ROOT\\DEFAULT"),  //namespace
				NULL,       // User name 
				NULL,       // User password
				0,         // Locale 
				NULL,     // Security flags
				0,         // Authority 
				0,        // Context object 
				&i_wbem_services);   // IWbemServices proxy

			if (FAILED(hr))
			{
				i_wbem_locator->Release();
			}
			else
			{
				// Set the proxy so that impersonation of the client occurs.
				hr = CoSetProxyBlanket(i_wbem_services,
					RPC_C_AUTHN_WINNT,
					RPC_C_AUTHZ_NONE,
					NULL,
					RPC_C_AUTHN_LEVEL_CALL,
					RPC_C_IMP_LEVEL_IMPERSONATE,
					NULL,
					EOAC_NONE
				);

				if (FAILED(hr))
				{
					i_wbem_services->Release();
					i_wbem_locator->Release();
				}
				else
				{
					wmi_initialized = true;
				}
			}
		}
	}
}

GuruLib::WindowsManagementInstrumentation::~WindowsManagementInstrumentation()
{
	if (wmi_initialized)
	{
		i_wbem_services->Release();
		i_wbem_locator->Release();
	}
}