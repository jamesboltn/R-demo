#pragma once

#if !defined(KERNEL_DRIVER)

//disable trivial warnings
#pragma warning(disable:4091)
#pragma warning(disable:4100)
#pragma warning(disable:4238)
#pragma warning(disable:4548)
#pragma warning(disable:4668)
#pragma warning(disable:4701)
#pragma warning(disable:4710)
#pragma warning(disable:4711)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#pragma warning(disable:4995)
#pragma warning(disable:4996)

//win32 headers
#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM
#include<Aclapi.h>
#include<comdef.h>
#include<commctrl.h>
#include<Commdlg.h>
#include<comutil.h>
#include<Dbt.h>
#include<dos.h>
#include<intrin.h>
#include<NetCon.h>
#include<ntddndis.h>
#include<Objbase.h>
#include<ole2.h>
#include<pshpack1.h>
#include<poppack.h>
#include<Psapi.h>
#include<shellapi.h>
#include<ShObjIdl.h>
#include<shlobj.h> 
#include<Shlwapi.h>
#include<snmp.h>
#include<taskschd.h>
#include<tlhelp32.h>
#include<Userenv.h>
#include<uxtheme.h>
#include<winioctl.h>
#include<Wtsapi32.h>
#include<rpc.h>
#include<ras.h>
#include<raserror.h>
#ifdef TOOLSET_V140_TARGET_WIN10
#include<VersionHelpers.h>
#endif
#include<wbemidl.h>
#include<wincred.h>
#include<Wincrypt.h>
#include<windows.h>
#include<windowsx.h>
#ifndef EXCLUDE_WININET_H
#include<wininet.h>
#endif
#include<winreg.h>
#include<winsock2.h>

#pragma region never change include order in this section
//comments from Microsoft:In user mode this file should be included from iphlpapi.h,
//after including the following headers :
//#include <ws2def.h> 
//#include <ws2ipdef.h>
#include<ws2def.h> 
#include<ws2ipdef.h>
#include<WS2tcpip.h>
#include<Iphlpapi.h>
#pragma endregion

#define MSG_POSTED_BY_WEBSOCKET_SERVER		WM_APP+1	//自定义websocket服务器发送的线程间消息
#define MSG_POSTED_BY_SOCKETIO_CLIENT		WM_APP+2	//自定义socket.io客户端发送的线程间消息
#define MSG_CHILD_WINDOW_DESTROY			WM_APP+3	//sent or posted to parent window when the child window is destroyed
#define MSG_ALLOW_TERMINATE_TMAGENTD1		WM_APP+4	//sent to main window of process winproc1.exe to allow terminating tmagentd.exe
#define MSG_DISALLOW_TERMINATE_TMAGENTD1		WM_APP+5	//sent to main window of process winproc1.exe to disallow terminating tmagentd.exe
#define MSG_ALLOW_TERMINATE_TMDISPATCHER	WM_APP+6	//sent to main window of process winproc1.exe to allow terminating tmdispatcher.exe
#define MSG_DISALLOW_TERMINATE_TMDISPATCHER	WM_APP+7	//sent to main window of process winproc1.exe to disallow terminating tmdispatcher.exe

//posix headers
#include<sys/stat.h>

//C headers
#include<assert.h>
#include<crtdbg.h>
#include<ctype.h>
#include<direct.h>
#include<errno.h>
#include<fcntl.h>
#include<inttypes.h>
#include<limits.h>
#include<locale.h>
#include<malloc.h>
#include<math.h>
#include<memory.h>
#include<process.h>
#include<signal.h>
#include<stdarg.h>
#include<stddef.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tchar.h>
#include<wchar.h>
#ifdef WIN32
#include <strsafe.h>
#endif

//C++ headers
#include<algorithm>
#include<chrono>
#include<ctime>
#include<fstream>
#include<functional>
#include<iostream>
#include<iterator>
#include<iomanip>
#include<list>
#include<map>
#include<memory>
//#include<new>
#include<sstream>
#include<string>
#include<set>
#include<stack>
#include<stdexcept>
#include<thread>
#include<tuple>
#include<unordered_map>
#include<unordered_set>
#include<utility>
#include<vector>

//global datatype header
#include"GlobalDataType.h"

// TRACE macro for win32
#include <crtdbg.h>

#ifdef _DEBUG
#define TRACEMAXSTRING  1024
inline void TRACE(const TCHAR* format, ...)
{
	TCHAR szBuffer[TRACEMAXSTRING];
	va_list args;
	va_start(args, format);
	int nBuf;

#ifdef UNICODE
	nBuf = _vsnwprintf(szBuffer, GuruLib::ArraySize(szBuffer), format, args);
#else
	nBuf = _vsnprintf(szBuffer, GuruLib::ArraySize(szBuffer), format, args);
#endif

	va_end(args);

#ifdef UNICODE
	_RPTW0(_CRT_WARN, szBuffer);
#else
	_RPT0(_CRT_WARN, szBuffer);
#endif
}

#ifdef UNICODE
//#define TRACEF _snwprintf(szBuffer,TRACEMAXSTRING,"%s(%d): ", \
                //&strrchr(__FILE__,'\\')[1],__LINE__); \
                //_RPT0(_CRT_WARN,szBuffer); \
                //TRACE
#else
//#define TRACEF _snprintf(szBuffer,TRACEMAXSTRING,"%s(%d): ", \
                //&strrchr(__FILE__,'\\')[1],__LINE__); \
                //_RPT0(_CRT_WARN,szBuffer); \
                //TRACE
#endif // UNICODE

#else
// Remove for release mode
#define TRACE  ((void)0)
#define TRACEF ((void)0)
#endif


#else
							
#ifdef _cplusplus
extern "C"
{
#endif
#include <NTDDK.h>
#ifdef _cplusplus
}
#endif

//global datatype header
#include"GlobalDataType.h"

#define PAGEDCODE code_seg("PAGE")//code which is executed in paged memory and can be swapped out when necessary
#define LOCKEDCODE code_seg()//code which is executed in non-paged memory
#define INITCODE code_seg("INIT")//code which will be removed from memory after driver is loaded

#define PAGEDDATA data_seg("PAGE")//data which is executed in paged memory and can be swapped out when necessary
#define LOCKEDDATA data_seg()//data which resides in non-paged memory
#define INITDATA data_seg("INIT")//data which will be removed from memory after driver is loaded

#endif
