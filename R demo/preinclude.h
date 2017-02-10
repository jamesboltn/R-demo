#pragma once

//enable comctl32.dll version 6 visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

//disable specific warnings
#pragma warning(disable:4820)
#pragma warning(disable:4710)
#pragma warning(disable:4917)
#pragma warning(disable:4668)
#pragma warning(disable:4100)
#pragma warning(disable:4701)
#pragma warning(disable:4711)
#pragma warning(disable:4548)
#pragma warning(disable:4238)
#pragma warning(disable:4996)
#pragma warning(disable:4091)
//win32 header
#define WIN32_LEAN_AND_MEAN
#include<winsock2.h>
#include<windows.h>
#include<windowsx.h>
#include<ShObjIdl.h>
#include<shlobj.h> 
#include"Shlwapi.h"
#include<commctrl.h >
#include<Commdlg.h>
#include<Objbase.h>



//C header
#include<assert.h>
#include<stdio.h>
#include<malloc.h>
#include<tchar.h>
#include<wchar.h>
#include<direct.h>
#include<stdlib.h>
#include<memory.h>
#include<ctype.h>

//C++ header
#include<algorithm>
#include<cstdlib>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<tuple>
#include<unordered_map>
#include<set>
#include<unordered_set>
#include<iterator>
#include<ctime>
#include<cstring>
#include<iomanip>
#include<stack>
#include<memory>
#include<functional>
#include<cmath>
#include<utility>
#include<cstdarg>

//global datatype header
#include"GlobalDataType.h"
