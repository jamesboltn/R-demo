#pragma once
#include"preinclude.h"
#include"Utilities.h"
#include"ClientDataType.h"

#define DIRECTORY_SEPARATOR_W		L"\\"
#define DIRECTORY_SEPARATOR_C_W		L'\\'
#define DIRECTORY_SEPARATOR			"\\"
#define DIRECTORY_SEPARATOR_C		'\\'
const wchar_t* SINGLE_INSTANCE(L"SINGLE_INSTANCE");
const wchar_t KEY_STARTUP[]({ L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" });

bool OS::Reboot(int time_out)
{
	::system("c:\\windows\\system32\\shutdown /g");
	return true;
	/*the commentted code provide reboot functionality which works only when the user of caller is active, thus is not applicable to a trading system*/
	//HANDLE hToken;
	//TOKEN_PRIVILEGES tkp;

	//if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	//{
	//	return false;
	//}
	//
	//LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	//tkp.PrivilegeCount = 1;
	//tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	//if (ExitWindowsEx(EWX_REBOOT, 0) == 0)
	//{
	//	return false;
	//}
	//
	////reset the previlages
	//tkp.Privileges[0].Attributes = 0;
	//AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
}

bool OS::StopShutDown()
{
	/*if (::AbortSystemShutdown(L"\\127.0.0.1") == 0)
	return false;
	else */
	return true;
}

bool OS::ShutDown(int time_out)
{
	::system("c:\\windows\\system32\\shutdown /s");
	return true;
	/*the commentted code provide shut down functionality which works only when the user of caller is active, thus is not applicable to a trading system*/
	//bool retval(true);
	//HANDLE hToken;
	//TOKEN_PRIVILEGES tkp;
	//if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	//{
	//	return false;
	//}
	//
	//LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	//tkp.PrivilegeCount = 1;
	//tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	//retval = ExitWindowsEx(EWX_POWEROFF, 0);

	//reset the previlages
	//tkp.Privileges[0].Attributes = 0;
	//AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	//return retval;
}

bool OS::IsAlreadyRunning()
{
	CreateEvent(NULL, FALSE, FALSE, SINGLE_INSTANCE);
	return ERROR_ALREADY_EXISTS == ::GetLastError();
}

//bool OS::BootOnStartUp( wchar_t* boot_name)
//{
//	wchar_t strFilePath[MAX_PATH];
//
//	if (::GetModuleFileNameW(NULL, strFilePath, MAX_PATH) != 0)
//	{
//		HKEY hRootKey(HKEY_LOCAL_MACHINE);
//		HKEY hKey(0);
//
//		HRESULT hr(RegOpenKey(hRootKey, KEY_STARTUP, &hKey));
//
//		if (hr == ERROR_SUCCESS)
//		{
//			hr = RegSetValueEx(hKey, boot_name, 0, REG_SZ, (const BYTE*)strFilePath, wcslen(strFilePath));
//
//			RegCloseKey(hKey);
//		}
//
//		return (hr == ERROR_SUCCESS);
//	}
//}

//bool OS::RemoveBootOnStartUp( wchar_t* boot_name)
//{
//	HKEY hRootKey(HKEY_LOCAL_MACHINE);
//
//	HKEY hKey(0);
//	HRESULT hr(RegOpenKey(hRootKey, KEY_STARTUP, &hKey));
//
//	if (hr == ERROR_SUCCESS)
//	{
//		hr = RegDeleteValue(hKey, boot_name);
//
//		RegCloseKey(hKey);
//	}
//
//	return (hr == ERROR_SUCCESS);
//}

OS::InstanceLimit::InstanceLimit(int limit /*= 1*/, wchar_t* name /*= NULL*/)
{
	assert(limit <= MAX_INSTANCES);
	instance_count = limit;
	std::wstring str(L"InstanceLimit");
	if (name)
		str = name;
	for (int j = 0; j < instance_count; ++j)
	{
		str += L"_";
		wchar_t buf[20];
		_itow_s(j, buf, ARRAYCOUNT(buf), 10);
		str += buf;
		names.push_back(str);
	}
	this_handle = NULL;
	current_instance_index = 0;
}

OS::InstanceLimit::~InstanceLimit()
{
	::CloseHandle(this_handle);
}

bool OS::InstanceLimit::IsExceedLimit()
{
	bool exceed_limit = true;
	HANDLE h = NULL;
	for (int j = 0; j < instance_count; ++j)
	{
#pragma warning(push)
#pragma warning(disable:4365)
		h = ::CreateMutex(NULL, FALSE, names[j].c_str());
#pragma warning(pop)
		assert(h);
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			::CloseHandle(h);
		}
		else
		{
			current_instance_index = j + 1;
			this_handle = h;
			exceed_limit = false;
			break;
		}
	}
	return exceed_limit;
}

std::wstring OS::CommonDirectory::GetProgramFilesDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{

		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetWindowsDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(::SHGetFolderPath(NULL, CSIDL_WINDOWS | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetSystemDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(::SHGetFolderPath(NULL, CSIDL_SYSTEM | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetMyDocumentsDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(::SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetAppDataDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetLocalAppDataDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetDesktopDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

std::wstring OS::CommonDirectory::GetStartupDirectoryW()
{
	std::wstring strDirectory(L"");

	wchar_t strPath[MAX_PATH];

	HRESULT hr(SHGetFolderPath(NULL, CSIDL_STARTUP | CSIDL_FLAG_CREATE, NULL, 0, strPath));

	if (SUCCEEDED(hr))
	{
		strDirectory = strPath;
	}

	return strDirectory;
}

void OS::CommonDirectory::GetExeDirW(std::wstring* dir)
{
#ifdef _DEBUG
	*dir = debug_exe_dir_w;
#else
	wchar_t strFilePath[MAX_PATH];
	*dir = L"";

	if (::GetModuleFileNameW(NULL, strFilePath, MAX_PATH) != 0)
	{
		*dir = strFilePath;
		*dir = dir->substr(0, dir->rfind(DIRECTORY_SEPARATOR_W));
	}
#endif
}

void OS::CommonDirectory::GetExeDirA(std::string* dir)
{
#ifdef _DEBUG
	*dir = debug_exe_dir;
#else
	char strFilePath[MAX_PATH];
	*dir = "";

	if (::GetModuleFileNameA(NULL, strFilePath, MAX_PATH) != 0)
	{
		*dir = strFilePath;
		*dir = dir->substr(0, dir->rfind(DIRECTORY_SEPARATOR));
	}
#endif
}