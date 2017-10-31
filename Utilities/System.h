#pragma once
#define TOOLSET_V140_TARGET_WIN10 //define this symbol to include "versionhelpers.h"
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace System
	{
		LIB bool Reboot(int time_out = 0);
		LIB bool ShutDown(int time_out = 0);
		LIB void GetHostName(char* host_name, int length);
		LIB void GetSystemName(std::wstring &os_name);
		LIB void GetCurrentVersionNumber(std::wstring &version_number);
		LIB DWORD GetBuildNumber();
		LIB DWORD GetMajorVersionNumber();
		namespace Screen
		{
			//	inline int GetScreenWidth();
			//	inline int GetScreenHeight();
		}
		namespace Process
		{
			LIB bool IsProcessRunning(wchar_t* module_name);
			LIB BOOL IsProcessRunning(DWORD process_id);
			LIB BOOL IsProcessRunning(HANDLE process_handle);
			LIB BOOL IsCurrentProcessElevated();
			LIB int  GetDuplicateRunningProcessesCount(wchar_t* module_name);
			LIB void GetProcessHandlesByModuleName(wchar_t* module_name, std::vector<HANDLE> &proc_handles);
			LIB void GetProcessNameByID(DWORD process_id, std::wstring &process_name);
			LIB BOOL IsWow64Process();
		}
		namespace Account
		{
			LIB BOOL IsRunningUnderLocalSystemAccount();
		}
	}
}


