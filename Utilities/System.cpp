#pragma once
#include"System.h"
#include"Registry.h"

static BOOL SetPrivilege(
	HANDLE hToken,          // token handle
	LPCTSTR Privilege,      // Privilege to enable/disable
	BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

	if (!LookupPrivilegeValue(NULL, Privilege, &luid)) return FALSE;

	// 
	// first pass.  get current privilege setting
	// 
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		&tpPrevious,
		&cbPrevious
	);

	if (GetLastError() != ERROR_SUCCESS) return FALSE;

	// 
	// second pass.  set privilege based on previous setting
	// 
	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;

	if (bEnablePrivilege) {
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	}
	else {
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
			tpPrevious.Privileges[0].Attributes);
	}

	AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tpPrevious,
		cbPrevious,
		NULL,
		NULL
	);

	if (GetLastError() != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

//static BOOL SetPrivilege( 
//HANDLE hToken,  // token handle 
//LPCTSTR Privilege,  // Privilege to enable/disable 
//BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable 
//) 
//{ 
//TOKEN_PRIVILEGES tp = { 0 }; 
//// Initialize everything to zero 
//LUID luid; 
//DWORD cb=sizeof(TOKEN_PRIVILEGES); 
//if(!LookupPrivilegeValue( NULL, Privilege, &luid ))
//return FALSE; 
//tp.PrivilegeCount = 1; 
//tp.Privileges[0].Luid = luid; 
//if(bEnablePrivilege) { 
//tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
//} else { 
//tp.Privileges[0].Attributes = 0; 
//} 
//AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL ); 
//if (GetLastError() != ERROR_SUCCESS) 
//return FALSE; 
//
//return TRUE;
//}

HANDLE EnableDebugPrivilege()
{
	HANDLE hToken;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			if (!ImpersonateSelf(SecurityImpersonation))
				return NULL;

			if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)) {
				//DisplayError(L"OpenThreadToken");
				return NULL;
			}
		}
		else
			return NULL;
	}

	// enable SeDebugPrivilege
	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
	{
		//DisplayError(L"SetPrivilege");

		// close token handle
		CloseHandle(hToken);

		// indicate failure
		return NULL;
	}

	return hToken;
}

void DisableDebugPrivilege(HANDLE token)
{
	if (token != NULL)
	{
		// disable SeDebugPrivilege
		SetPrivilege(token, SE_DEBUG_NAME, FALSE);
		// close handles
		CloseHandle(token);
	}
}

bool GuruLib::System::Reboot(int time_out)
{
	::system("c:\\windows\\system32\\shutdown /g");
	return true;
}

bool GuruLib::System::ShutDown(int time_out)
{
	::system("c:\\windows\\system32\\shutdown /s");
	return true;
}

bool GuruLib::System::Process::IsProcessRunning(wchar_t* module_name)
{
	HANDLE processesSnapshot;
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	processesSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// Check whether to stop the service.
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	Process32First(processesSnapshot, &processInfo);
	if (wcscmp(module_name, processInfo.szExeFile) == 0)
	{
		::CloseHandle(processesSnapshot);
		return true;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (wcscmp(module_name, processInfo.szExeFile) == 0)
		{
			::CloseHandle(processesSnapshot);
			return true;
		}
	}
	return false;
}

BOOL GuruLib::System::Process::IsProcessRunning(DWORD process_id)
{
	HANDLE process = ::OpenProcess(SYNCHRONIZE, FALSE, process_id);
	DWORD ret = ::WaitForSingleObject(process, 0);
	::CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

LIB BOOL GuruLib::System::Process::IsProcessRunning(HANDLE process_handle)
{
	DWORD ret = ::WaitForSingleObject(process_handle, 0);
	return ret == WAIT_TIMEOUT;
}

BOOL GuruLib::System::Process::IsCurrentProcessElevated()
{
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (::GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
		{
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken)
	{
		::CloseHandle(hToken);
	}
	return fRet;
}

BOOL GuruLib::System::Process::IsWow64Process()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	LPFN_ISWOW64PROCESS fnIsWow64Process;

	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(::GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

void GuruLib::System::Process::GetProcessHandlesByModuleName(wchar_t* module_name, std::vector<HANDLE> &proc_handles)
{
	HANDLE token = EnableDebugPrivilege();

	proc_handles.clear();
	HANDLE processesSnapshot;
	HANDLE process_handle;
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	processesSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// Check whether to stop the service.
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		if (token != NULL)
		{
			DisableDebugPrivilege(token);
		}
		return;
	}

	Process32First(processesSnapshot, &processInfo);
	if (wcscmp(module_name, processInfo.szExeFile) == 0)
	{
		process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);
		if (process_handle != NULL)
		{
			proc_handles.push_back(process_handle);
		}
		//::CloseHandle(processesSnapshot);
		//return true;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (wcscmp(module_name, processInfo.szExeFile) == 0)
		{
			process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);
			if (process_handle != NULL)
			{
				proc_handles.push_back(process_handle);
			}
			else
			{
			}
			//::CloseHandle(processesSnapshot);
			//return true;
		}
	}
	::CloseHandle(processesSnapshot);
	if (token != NULL)
	{
		DisableDebugPrivilege(token);
	}
}

void GuruLib::System::Process::GetProcessNameByID(DWORD process_id, std::wstring &process_name)
{
	HANDLE token = EnableDebugPrivilege();
	HANDLE hProc;
	wchar_t szProcessName[80];
	HMODULE ahMod[10];
	DWORD dwNeeded;

	hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
	if (hProc)
	{
		if (::EnumProcessModules(hProc, ahMod, sizeof(ahMod), &dwNeeded))
		{
			if (::GetModuleBaseName(hProc, ahMod[0], szProcessName, GuruLib::ArraySize(szProcessName)))
			{
				//success
				process_name = szProcessName;
			}
			else
			{
				//failure
			}
		}
		CloseHandle(hProc);
	}
	if (token != NULL)
	{
		DisableDebugPrivilege(token);
	}
}

int  GuruLib::System::Process::GetDuplicateRunningProcessesCount(wchar_t* module_name)
{
	int count = 0;
	HANDLE processesSnapshot;
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	processesSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// Check whether to stop the service.
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		return count;
	}

	Process32First(processesSnapshot, &processInfo);
	if (wcscmp(module_name, processInfo.szExeFile) == 0)
	{
		//::CloseHandle(processesSnapshot);
		//return true;
		++count;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (wcscmp(module_name, processInfo.szExeFile) == 0)
		{
			++count;
			//::CloseHandle(processesSnapshot);
			//return true;
		}
	}
	::CloseHandle(processesSnapshot);
	return count;
}

void GuruLib::System::GetHostName(char* host_name, int length)
{
	::gethostname(host_name, length);
}

void GuruLib::System::GetSystemName(std::wstring &os_name)
{
	GuruLib::Registry reg(HKEY_LOCAL_MACHINE);
	if (reg.SetKey(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"))
	{
		reg.ReadString(L"ProductName", L"", os_name);
	}
}

void GuruLib::System::GetCurrentVersionNumber(std::wstring &version_number)
{
	GuruLib::Registry reg(HKEY_LOCAL_MACHINE);
	if (reg.SetKey(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"))
	{
		reg.ReadString(L"CurrentVersion", L"", version_number);
	}
}

DWORD GuruLib::System::GetBuildNumber()
{
	HINSTANCE h_ntdll_dll = LoadLibrary(L"ntdll.dll");
	if (h_ntdll_dll == NULL)return 0;

	typedef NTSTATUS(*RtlGetVersionProc)(PRTL_OSVERSIONINFOW);
	RtlGetVersionProc _RtlGetVersionProc = (RtlGetVersionProc)GetProcAddress(h_ntdll_dll, "RtlGetVersion");
	if (_RtlGetVersionProc == NULL)return 0;

	RTL_OSVERSIONINFOW rtl_os_version_info;
	(*_RtlGetVersionProc)(&rtl_os_version_info);
	return rtl_os_version_info.dwBuildNumber;
}

DWORD GuruLib::System::GetMajorVersionNumber()
{
	HINSTANCE h_ntdll_dll = LoadLibrary(L"ntdll.dll");
	if (h_ntdll_dll == NULL)return 0;

	typedef NTSTATUS(*RtlGetVersionProc)(PRTL_OSVERSIONINFOW);
	RtlGetVersionProc _RtlGetVersionProc = (RtlGetVersionProc)GetProcAddress(h_ntdll_dll, "RtlGetVersion");
	if (_RtlGetVersionProc == NULL)return 0;

	RTL_OSVERSIONINFOW rtl_os_version_info;
	(*_RtlGetVersionProc)(&rtl_os_version_info);
	return rtl_os_version_info.dwMajorVersion;
}

BOOL GuruLib::System::Account::IsRunningUnderLocalSystemAccount()
{
	HANDLE hToken;
	UCHAR bTokenUser[sizeof(TOKEN_USER) + 8 + 4 * SID_MAX_SUB_AUTHORITIES];
	PTOKEN_USER pTokenUser = (PTOKEN_USER)bTokenUser;
	ULONG cbTokenUser;
	SID_IDENTIFIER_AUTHORITY siaNT = SECURITY_NT_AUTHORITY;
	PSID pSystemSid;
	BOOL bSystem;

	// open process token
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_QUERY,
		&hToken))
		return FALSE;

	// retrieve user SID
	if (!GetTokenInformation(hToken, TokenUser, pTokenUser,
		sizeof(bTokenUser), &cbTokenUser))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	CloseHandle(hToken);

	// allocate LocalSystem well-known SID
	if (!AllocateAndInitializeSid(&siaNT, 1, SECURITY_LOCAL_SYSTEM_RID,
		0, 0, 0, 0, 0, 0, 0, &pSystemSid))
		return FALSE;

	// compare the user SID from the token with the LocalSystem SID
	bSystem = EqualSid(pTokenUser->User.Sid, pSystemSid);

	FreeSid(pSystemSid);

	return bSystem;
}