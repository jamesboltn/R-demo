#pragma once
#include"FileSystem.h"
#include"DateTime.h"
#include"Utilities.h"

BOOL GuruLib::FileSystem::Directory::Delete(const wchar_t* dir)
{
	std::vector<wchar_t> vec;
	for (int i = 0; dir[i] != L'\0'; ++i)
	{
		vec.push_back(dir[i]);
	}
	vec.push_back(L'\0');
	vec.push_back(L'\0');

	SHFILEOPSTRUCT sh;
	sh.hwnd = NULL;
	sh.wFunc = FO_DELETE;
	sh.pFrom = &vec[0];
	sh.pTo = NULL;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	return !::SHFileOperation(&sh);
}

BOOL GuruLib::FileSystem::Directory::Copy(const wchar_t* Path, wchar_t* PathDest)
{
	//wchar_t	Path[] = L"E:\\Demo\\Output\\x64\\Debug\\tmservice\\";
	//wchar_t PathDest[] = L"E:\\tmservice\\";

	wchar_t SubDir[MAX_PATH];
	wchar_t SubFile[MAX_PATH];

	WIN32_FIND_DATA fd = { 0 };
	wcscpy_s(SubDir, GuruLib::ArraySize(SubDir), Path);
	wcscat_s(SubDir, GuruLib::ArraySize(SubDir), L"*.*");
	HANDLE h = FindFirstFile(SubDir, &fd); //Win32 API

	if (h != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(h, &fd))            //Win32 API
		{
			wcscpy_s(SubFile, GuruLib::ArraySize(SubFile), fd.cFileName);

			if (wcscmp(SubFile, L".") == 0 || wcscmp(SubFile, L"..") == 0)
			{
				continue;
			}

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//FILE ATTRIBUTE IS A DIRECTORY:
				wcscpy_s(SubDir, GuruLib::ArraySize(SubDir), PathDest);
				wcscat_s(SubDir, GuruLib::ArraySize(SubDir), SubFile);
				if (!CreateDirectory(SubDir, NULL))    //VCL function
				{
					return FALSE;
					//sShowMessage("Cannot create directory :" + PathDest + File);
				}

				//the SubDir is used as SourceDir to reduce memory usage
				wcscpy_s(SubDir, GuruLib::ArraySize(SubDir), Path);
				wcscat_s(SubDir, GuruLib::ArraySize(SubDir), fd.cFileName);
				wcscat_s(SubDir, GuruLib::ArraySize(SubDir), L"\\");
				//the SubFile is used as DestDir to reduce memory usage
				wcscpy_s(SubFile, GuruLib::ArraySize(SubFile), PathDest);
				wcscat_s(SubFile, GuruLib::ArraySize(SubFile), fd.cFileName);
				wcscat_s(SubFile, GuruLib::ArraySize(SubFile), L"\\");

				//Recursive call
				GuruLib::FileSystem::Directory::Copy(SubDir, SubFile);
			}
			else
			{

				//FILE ATTRIBUTTE IS NOT A DIRECTORY (IS A FILE):
				//the SubDir is used as SourceDir to reduce memory usage
				wcscpy_s(SubDir, GuruLib::ArraySize(SubDir), Path);
				wcscat_s(SubDir, GuruLib::ArraySize(SubDir), fd.cFileName);
				//the SubFile is used as DestDir to reduce memory usage
				wcscpy_s(SubFile, GuruLib::ArraySize(SubFile), PathDest);
				SubFile[wcslen(SubFile) - 1] = L'\0';
				::CreateDirectory(SubFile, NULL);
				SubFile[wcslen(SubFile)] = L'\\';
				wcscat_s(SubFile, GuruLib::ArraySize(SubFile), fd.cFileName);

				if (!CopyFile(SubDir, SubFile, FALSE)) //Win32 API
				{
					//::GetLastError();
					//ShowMessage("Cannot copy file - errno = " + String(errno));
					return FALSE;
				}
			}
		}
		FindClose(h);  //Win 32 API
		return TRUE;
	}
	else
	{
		//ShowMessage("Error: FindFirstFile() returned an Invalid Handle");
		return FALSE;
	}
}

void GuruLib::FileSystem::File::WriteLog(const char* msg, FILE* fp, std::string* time_stamp)
{
	if (fp != NULL)
	{
		std::string record;
		if (time_stamp == NULL)
		{
			DateTime dt;
			record = dt.Current().ToDateTimeString() + " ";
		}
		else
		{
			record = *time_stamp + " ";
		}
		record = record + msg + "\n";
		fwrite(record.c_str(), sizeof(char), record.size(), fp);
	}
}

void GuruLib::FileSystem::File::GetFileMd5Hash(const char* file_path, OUT std::string &buffer)
{
#pragma region openssl/md5 implementation
	//buffer = "";
	//unsigned char c[MD5_DIGEST_LENGTH];
	//int i;
	//FILE *inFile = fopen(file_path, "rb");
	//MD5_CTX mdContext;
	//int bytes;
	//unsigned char data[1024];

	//if (inFile == NULL) 
	//{
	//	//printf("%s can't be opened.\n", filename);
	//	return;
	//}

	//MD5_Init(&mdContext);
	//while ((bytes = fread(data, 1, 1024, inFile)) != 0)
	//{
	//	MD5_Update(&mdContext, data, bytes);
	//}
	//	
	//MD5_Final(c, &mdContext);

	//char hash_unit[4];
	//for (i = 0; i < MD5_DIGEST_LENGTH; i++)
	//{
	//	//1551d1fed...
	//	sprintf_s(hash_unit, ArraySize(hash_unit), "%02x", c[i]);
	//	buffer += hash_unit;
	//	//printf("%02x", c[i]);
	//}
	//fclose(inFile);
#pragma endregion

#pragma region win32 implementation
#define BUFSIZE 1024
#define MD5LEN  16

	buffer = "";
	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";
	// Logic to check usage goes here.

	hFile = CreateFileA(file_path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		//printf("Error opening file %s\nError: %d\n", filename, dwStatus);
		return /*dwStatus*/;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		//printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		return /*dwStatus*/;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		//printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return /*dwStatus*/;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			//printf("CryptHashData failed: %d\n", dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return /*dwStatus*/;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		//printf("ReadFile failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return /*dwStatus*/;
	}

	cbHash = MD5LEN;
	char hash_unit[4];
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		//printf("MD5 hash of file %s is: ", filename);
		for (DWORD i = 0; i < cbHash; i++)
		{
			sprintf_s(hash_unit, GuruLib::ArraySize(hash_unit), "%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
			buffer += hash_unit;
			//printf("%c%c", rgbDigits[rgbHash[i] >> 4],rgbDigits[rgbHash[i] & 0xf]);
		}
		//printf("\n");
	}
	else
	{
		dwStatus = GetLastError();
		//printf("CryptGetHashParam failed: %d\n", dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);
#pragma endregion
}

bool GuruLib::FileSystem::File::GetFileVersionString(const wchar_t* file_path, OUT std::wstring &version_string)
{
	version_string.clear();

	DWORD size;
	DWORD handle = 0;
	size = GetFileVersionInfoSize(file_path, &handle);
	BYTE* versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(file_path, handle, size, versionInfo))
	{
		delete[] versionInfo;
		return false;
	}
	// we have version information
	UINT                len = 0;
	VS_FIXEDFILEINFO*   vsfi = NULL;
	VerQueryValue(versionInfo, L"\\", (void**)&vsfi, &len);

	wchar_t ver_string[100];
	swprintf_s(ver_string, GuruLib::ArraySize(ver_string), L"%d,%d,%d,%d", HIWORD(vsfi->dwFileVersionMS), LOWORD(vsfi->dwFileVersionMS), HIWORD(vsfi->dwFileVersionLS), LOWORD(vsfi->dwFileVersionLS));

	version_string = ver_string;
	delete[] versionInfo;
	return true;
}

bool GuruLib::FileSystem::File::HasVersionString(const wchar_t* file_path)
{
	DWORD size;
	DWORD handle = 0;
	size = GetFileVersionInfoSize(file_path, &handle);
	BYTE* versionInfo = new BYTE[size];
	if (!GetFileVersionInfo(file_path, handle, size, versionInfo))
	{
		delete[] versionInfo;
		return false;
	}
	else
	{
		delete[] versionInfo;
		return true;
	}

	//// we have version information
	//UINT                len = 0;
	//VS_FIXEDFILEINFO*   vsfi = NULL;
	//VerQueryValue(versionInfo, L"\\", (void**)&vsfi, &len);

	//wchar_t ver_string[100];
	//swprintf_s(ver_string, ArraySize(ver_string), L"%d,%d,%d,%d", HIWORD(vsfi->dwFileVersionMS), LOWORD(vsfi->dwFileVersionMS), HIWORD(vsfi->dwFileVersionLS), LOWORD(vsfi->dwFileVersionLS));

	//delete[] versionInfo;
	//return true;
}

std::wstring GuruLib::FileSystem::Directory::Common::GetMyDocumentsDirectoryW()
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

std::wstring GuruLib::FileSystem::Directory::Common::GetAppDataDirectoryW()
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

std::wstring GuruLib::FileSystem::Directory::Common::GetLocalAppDataDirectoryW()
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

std::wstring GuruLib::FileSystem::Directory::Common::GetDesktopDirectoryW()
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

std::wstring GuruLib::FileSystem::Directory::Common::GetStartupDirectoryW()
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

#define DIRECTORY_SEPARATOR_WSTRING		L"\\"
#define DIRECTORY_SEPARATOR_WCHAR		L'\\'
#define DIRECTORY_SEPARATOR_STRING			"\\"
#define DIRECTORY_SEPARATOR_CHAR		'\\'

void GuruLib::FileSystem::Directory::Common::GetExeDirW(std::wstring* dir)
{
	wchar_t strFilePath[MAX_PATH];
	*dir = L"";

	if (::GetModuleFileNameW(NULL, strFilePath, MAX_PATH) != 0)
	{
		*dir = strFilePath;
		*dir = dir->substr(0, dir->rfind(DIRECTORY_SEPARATOR_WSTRING));
	}
}
void GuruLib::FileSystem::Directory::Common::GetExeDirA(std::string* dir)
{
	char strFilePath[MAX_PATH];
	*dir = "";

	if (::GetModuleFileNameA(NULL, strFilePath, MAX_PATH) != 0)
	{
		*dir = strFilePath;
		*dir = dir->substr(0, dir->rfind(DIRECTORY_SEPARATOR_STRING));
	}
}
