#pragma once
#include"WindowsService.h"

/**************static definitions***********************/
bool GuruLib::WindowsService::InstallService(wchar_t* service_binary_path, wchar_t* service_name)
{
	SC_HANDLE handle_scm, handle_service;
	handle_scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (handle_scm == NULL)
	{
		return false;
	}
	handle_service = CreateService(
		handle_scm,
		service_name,
		service_name, // service name to display
		SERVICE_ALL_ACCESS, // desired access 
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
		SERVICE_AUTO_START, // start type 
		SERVICE_ERROR_NORMAL, // error control type 
		service_binary_path, // service's binary 
		NULL, // no load ordering group 
		NULL, // no tag identifier 
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL); // no password
	::CloseServiceHandle(handle_scm);
	if (handle_service == NULL)
	{
		return false;
	}
	::CloseServiceHandle(handle_service);
	return true;
}
bool GuruLib::WindowsService::DeleteService(wchar_t* service_name)
{
	SC_HANDLE handle_scm, handle_service;
	handle_scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (handle_scm == NULL)
	{
		return false;
	}

	handle_service = OpenService(handle_scm, service_name, SERVICE_ALL_ACCESS);
	::CloseServiceHandle(handle_scm);
	if (handle_service == NULL)
	{
		return false;
	}
	if (::DeleteService(handle_service) == FALSE)
	{
		return false;
	}
	::CloseServiceHandle(handle_service);
	return true;
}
bool GuruLib::WindowsService::ServiceExist(wchar_t* service_name)
{
	SC_HANDLE handle_scm = OpenSCManager(0, 0, GENERIC_READ);
	if (handle_scm == NULL)
	{
		return false;
	}
	SC_HANDLE handle_service = OpenService(handle_scm, service_name, GENERIC_READ);
	CloseServiceHandle(handle_scm);
	if (!handle_service)
	{
		if (ERROR_SERVICE_DOES_NOT_EXIST != GetLastError())
		{
			//Failed to OpenService()
			return false;
		}
		else
		{
			//Service does not exist
			return false;
		}
	}
	CloseServiceHandle(handle_service);
	return true;
}

/**************non-static definitions*************************************/
GuruLib::WindowsService::WindowsService(const wchar_t* _service_name) :
	service_name(_service_name)//assign service name 
{
	service_status = { 0 };
}

GuruLib::WindowsService::WindowsService()
{
	service_status = { 0 };
}

GuruLib::WindowsService::~WindowsService()
{
	if (handle_scm)
	{
		::CloseServiceHandle(handle_scm);
		handle_scm = NULL;
	}
}

bool GuruLib::WindowsService::Install(const wchar_t* service_binary_path, ServiceType _service_type)
{
	if (handle_scm == NULL)
	{
		return false;
	}
	DWORD service_type;
	if (_service_type == KERNEL_DRIVER_SERVICE)
	{
		service_type = SERVICE_KERNEL_DRIVER;
	}
	else
	{
		service_type = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	}
	SC_HANDLE handle_service = ::CreateService(
		handle_scm,
		service_name.c_str(),
		service_name.c_str(), // service name to display
		SERVICE_ALL_ACCESS, // desired access 
		service_type,
		SERVICE_AUTO_START, // start type 
		SERVICE_ERROR_NORMAL, // error control type 
		service_binary_path, // service's binary 
		NULL, // no load ordering group 
		NULL, // no tag identifier 
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL); // no password

	if (handle_service == NULL)
	{
		return false;
	}
	::CloseServiceHandle(handle_service);
	return true;
}
bool GuruLib::WindowsService::Delete()
{
	SC_HANDLE handle_service;
	if (handle_scm == NULL)
	{
		return false;
	}

	handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_ALL_ACCESS);
	if (handle_service == NULL)
	{
		return false;
	}

	SERVICE_STATUS service_status;
	BOOL ret = QueryServiceStatus(handle_service, &service_status);
	if (ret)
	{
		if (service_status.dwCurrentState == SERVICE_RUNNING)
		{
			if (!ControlService(handle_service, SERVICE_CONTROL_STOP, &service_status))
			{
				//report error
			}
		}
	}

	if (::DeleteService(handle_service) == FALSE)
	{
		return false;
	}
	::CloseServiceHandle(handle_service);
	return true;
}

bool GuruLib::WindowsService::Delete(const wchar_t* _service_name)
{
	SC_HANDLE handle_service;
	if (handle_scm == NULL)
	{
		return false;
	}

	handle_service = OpenService(handle_scm, _service_name, SERVICE_ALL_ACCESS);
	if (handle_service == NULL)
	{
		return false;
	}

	SERVICE_STATUS service_status;
	BOOL ret = QueryServiceStatus(handle_service, &service_status);
	if (ret)
	{
		if (service_status.dwCurrentState == SERVICE_RUNNING)
		{
			if (!ControlService(handle_service, SERVICE_CONTROL_STOP, &service_status))
			{
				//report error
			}
		}
	}

	if (::DeleteService(handle_service) == FALSE)
	{
		return false;
	}
	::CloseServiceHandle(handle_service);
	return true;
}

bool GuruLib::WindowsService::Exist()
{
	if (handle_scm == NULL)
	{
		return false;
	}

	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), GENERIC_READ);
	if (!handle_service)
	{
		if (ERROR_SERVICE_DOES_NOT_EXIST != GetLastError())
		{
			//Failed to OpenService()
			return false;
		}
		else
		{
			//Service does not exist
			return false;
		}
	}
	CloseServiceHandle(handle_service);
	return true;
}
bool GuruLib::WindowsService::Start()
{
	if (!handle_scm)
	{
		return false;
	}
	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_QUERY_STATUS | SERVICE_START);

	if (!handle_service)
	{
		//printf("OpenService failed");
		return false;
	}

	SERVICE_STATUS service_status;
	BOOL ret = QueryServiceStatus(handle_service, &service_status);
	if (ret)
	{
		if (service_status.dwCurrentState == SERVICE_RUNNING)
		{
			//printf("the service is running");
			CloseServiceHandle(handle_service);
			return true;
		}
		else
		{
			int ret = StartService(handle_service, NULL, NULL);
			if (!ret)
			{
				//printf("start service failed");
				CloseServiceHandle(handle_service);
				return false;
			}
			else
			{
				//TODO:add throttle respect code
				//printf("start service succeeded");
				CloseServiceHandle(handle_service);
				return true;
			}
		}
	}

	//query failed
	//printf("QueryServiceStatus failed");
	CloseServiceHandle(handle_service);
	return false;
}
bool GuruLib::WindowsService::Running()
{
	if (handle_scm == NULL)
	{
		return false;
	}

	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_QUERY_STATUS);
	if (handle_service == NULL)
	{
		return false;
	}

	SERVICE_STATUS service_status;
	BOOL ret = QueryServiceStatus(handle_service, &service_status);
	CloseServiceHandle(handle_service);
	if (ret)
	{
		if (service_status.dwCurrentState == SERVICE_RUNNING)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//query failed
	return false;
}

LIB bool GuruLib::WindowsService::Stop()
{
	if (handle_scm == NULL)
	{
		return false;
	}

	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (handle_service == NULL)
	{
		//OpenService failed
		return false;
	}

	SERVICE_STATUS service_status;
	BOOL ret = QueryServiceStatus(handle_service, &service_status);
	if (ret)
	{
		if (service_status.dwCurrentState != SERVICE_STOPPED && service_status.dwCurrentState != SERVICE_STOP_PENDING
)
		{
			//If the service is not stopped or not stop_pending
			if (!ControlService(handle_service, SERVICE_CONTROL_STOP, &service_status))
			{
				//Stop service failed
				CloseServiceHandle(handle_service);
				auto ret = GetLastError();

				return false;
			}
			else
			{
				//Stop service succeeded
				CloseServiceHandle(handle_service);
				return true;
			}
		}
		else
		{
			//The service is not running
			CloseServiceHandle(handle_service);
			return true;
		}
	}

	//QueryServiceStatus failed
	CloseServiceHandle(handle_service);
	return false;
}

BOOL GuruLib::WindowsService::ReportStatus(unsigned long status)
{
	current_status = status;
	service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status.dwCurrentState = status;
	service_status.dwControlsAccepted = (status == SERVICE_START_PENDING) ? 0 : SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	service_status.dwWin32ExitCode = NO_ERROR;
	service_status.dwServiceSpecificExitCode = 0;
	service_status.dwCheckPoint = 0;
	service_status.dwWaitHint = 0;
	return ::SetServiceStatus(service_status_handle, &service_status);
}

BOOL GuruLib::WindowsService::ReportStatus()
{
	service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status.dwCurrentState = current_status;
	service_status.dwControlsAccepted = (current_status == SERVICE_START_PENDING) ? 0 : SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	service_status.dwWin32ExitCode = NO_ERROR;
	service_status.dwServiceSpecificExitCode = 0;
	service_status.dwCheckPoint = 0;
	service_status.dwWaitHint = 0;
	return ::SetServiceStatus(service_status_handle, &service_status);
}

bool GuruLib::WindowsService::Enable()
{
	if (handle_scm == NULL)
	{
		return false;
	}

	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_CHANGE_CONFIG);
	if (handle_service == NULL)
	{
		return false;
	}

	int ret = ChangeServiceConfig(handle_service, SERVICE_NO_CHANGE, SERVICE_AUTO_START, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	CloseServiceHandle(handle_service);

	return ret != 0;
}

bool GuruLib::WindowsService::Disable()
{
	if (handle_scm == NULL)
	{
		return false;
	}

	SC_HANDLE handle_service = OpenService(handle_scm, service_name.c_str(), SERVICE_CHANGE_CONFIG);
	if (handle_service == NULL)
	{
		return false;
	}

	int ret = ChangeServiceConfig(handle_service, SERVICE_NO_CHANGE, SERVICE_DISABLED, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	CloseServiceHandle(handle_service);

	return ret != 0;
}

BOOL GuruLib::WindowsService::LaunchApplicationFromUserSession(const wchar_t* module_path, wchar_t* command_line)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bResult = FALSE;
	DWORD dwSessionId, winlogonPid;
	HANDLE hUserToken, hUserTokenDup, hPToken, hProcess;
	DWORD dwCreationFlags;

	// Log the client on to the local computer.

	dwSessionId = WTSGetActiveConsoleSessionId();

	//////////////////////////////////////////
	// Find the winlogon process
	////////////////////////////////////////

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		return 1;
	}

	do
	{
		if (wcscmp(procEntry.szExeFile, L"winlogon.exe") == 0)
		{
			// We found a winlogon process...
			// make sure it's running in the console session
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId)
				&& winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	////////////////////////////////////////////////////////////////////////

	WTSQueryUserToken(dwSessionId, &hUserToken);
	dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = L"winsta0\\default";
	ZeroMemory(&pi, sizeof(pi));
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY
		| TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID
		| TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		int abcd = GetLastError();
		//printf("Process token open Error: %u\n", GetLastError());
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		//printf("Lookup Privilege value Error: %u\n", GetLastError());
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL,
		SecurityIdentification, TokenPrimary, &hUserTokenDup);
	int dup = GetLastError();

	//Adjust Token privilege
	SetTokenInformation(hUserTokenDup,
		TokenSessionId, (void*)dwSessionId, sizeof(DWORD));

	if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL, NULL))
	{
		int abc = GetLastError();
		//printf("Adjust Privilege value Error: %u\n", GetLastError());
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		//printf("Token does not have the provilege\n");
	}

	LPVOID pEnv = NULL;

	if (::CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
		pEnv = NULL;

	// Launch the process in the client's logon session.
	bResult = CreateProcessAsUser(
		hUserTokenDup,                     // client's access token
		module_path,    // file to execute
		command_line,                 // command line
		NULL,            // pointer to process SECURITY_ATTRIBUTES
		NULL,               // pointer to thread SECURITY_ATTRIBUTES
		FALSE,              // handles are not inheritable
		dwCreationFlags,     // creation flags
		pEnv,               // pointer to new environment block
		NULL,               // name of current directory
		&si,               // pointer to STARTUPINFO structure
		&pi                // receives information about new process
	);
	// End impersonation of client.

	//GetLastError Shud be 0

	int iResultOfCreateProcessAsUser = GetLastError();

	//Perform All the Close Handles tasks

	CloseHandle(hProcess);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);
}