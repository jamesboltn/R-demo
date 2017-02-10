#pragma once
#include"preinclude.h"
#include"Utilities.h"
#include"ClientDataType.h"

/*********begin global functions inside this class*********/
//filters files of which the file name contains "." or ".."
bool ContainsDots(wchar_t* file_name)
{
	if (lstrcmp(file_name, L".") && lstrcmp(file_name, L"..")) return FALSE;
	return true;
}
/**********end global functions inside this class**********/

void FileIO::WriteLog(const char* msg, FILE* fp, std::string* time_stamp)
{
	if (fp != NULL)
	{
		std::string record;
		if (time_stamp == NULL)
		{
			record = DateTime::GetCurrentDateTimeString() + " ";
		}
		else
		{
			record = *time_stamp + " ";
		}
		record = record + msg + "\n";
		fwrite(record.c_str(), sizeof(char), record.size(), fp);
	}
}

void FileIO::AppendText(FILE* fp, char* msg)
{
	if (fp != NULL)
	{	
		std::string line = std::string(msg) + "\n";
		fwrite(line.c_str(), sizeof(char), line.size(), fp);
	}
}

bool FileIO::DirectoryExists(wchar_t* dir)
{
	DWORD attr(GetFileAttributes(dir));
	return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileIO::ClearDirectory(const wchar_t* dir, const wchar_t* dir_same_with_prior_param)
{
	WIN32_FIND_DATA file_data;
	HANDLE file_handle;

	wchar_t* pattern1(L"\\*");
	wchar_t pattern[MAX_PATH];
	wcscpy_s(pattern, ARRAYCOUNT(pattern), dir);
	wcscat_s(pattern, ARRAYCOUNT(pattern), pattern1);

	wchar_t* pattern2(L"\\");
	wchar_t file_path[MAX_PATH];
	wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
	wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);

	file_handle = FindFirstFileW(pattern, &file_data);
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		bool bSearch(true);
		while (bSearch)
		{ //traverse the file system tree
			if (FindNextFile(file_handle, &file_data))
			{
				if (ContainsDots(file_data.cFileName)) continue;
				wcscat_s(file_path, ARRAYCOUNT(file_path), file_data.cFileName);
				if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!ClearDirectory(file_path, dir_same_with_prior_param))
					{
						FindClose(file_handle);
						return false;
					}
					RemoveDirectory(file_path); //clear empty subfolders
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
				else
				{
					if (file_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
						SetFileAttributes(file_data.cFileName, file_data.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
					if (!DeleteFile(file_path))
					{  // delete the file traversed
						FindClose(file_handle);
						return false;
					}
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
			}
			else
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
					bSearch = false;
				else
				{
					FindClose(file_handle);
					return false;
				}
			}
		}
		FindClose(file_handle);
		if (dir != dir_same_with_prior_param)
		{
			if (RemoveDirectory(dir))
			{
				return true; 
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	return false;

}

bool FileIO::ClearDirectory(wchar_t* dir)
{
	WIN32_FIND_DATA file_data;
	HANDLE file_handle;

	wchar_t* pattern1(L"\\*");
	wchar_t pattern[MAX_PATH];
	wcscpy_s(pattern, ARRAYCOUNT(pattern), dir);
	wcscat_s(pattern, ARRAYCOUNT(pattern), pattern1);

	wchar_t* pattern2(L"\\");
	wchar_t file_path[MAX_PATH];
	wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
	wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);

	file_handle = FindFirstFile(pattern, &file_data);
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		bool bSearch(true);
		while (bSearch)
		{ //traverse file system tree
			if (FindNextFile(file_handle, &file_data))
			{
				if (ContainsDots(file_data.cFileName)) continue;
				wcscat_s(file_path, ARRAYCOUNT(file_path), file_data.cFileName);
				if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!ClearDirectory(file_path))
					{
						FindClose(file_handle);
						return false;
					}
					RemoveDirectory(file_path); //clear empty subfolders
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
				else
				{
					if (file_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
						SetFileAttributes(file_data.cFileName, file_data.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
					if (!DeleteFile(file_path))
					{  // delete the file traversed
						FindClose(file_handle);
						return false;
					}
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
			}
			else
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
					bSearch = false;
				else
				{
					FindClose(file_handle);
					return false;
				}
			}
		}
		FindClose(file_handle);
		if (RemoveDirectory(dir))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool FileIO::DeleteFilesInDir(wchar_t* dir, std::unordered_set<wchar_t*>* file_names)
{
	WIN32_FIND_DATA file_data;
	HANDLE file_handle;

	wchar_t* pattern1(L"\\*");
	wchar_t pattern[1024];
	wcscpy_s(pattern, ARRAYCOUNT(pattern), dir);
	wcscat_s(pattern, ARRAYCOUNT(pattern), pattern1);

	wchar_t* pattern2(L"\\");
	wchar_t file_path[1024];
	wcscpy_s(file_path, 1024, dir);
	wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);

	file_handle = FindFirstFile(pattern, &file_data);
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		bool bSearch(true);
		while (bSearch)
		{ //traverse file system tree
			if (FindNextFile(file_handle, &file_data))
			{
				if (ContainsDots(file_data.cFileName)) continue;
				wcscat_s(file_path, ARRAYCOUNT(file_path), file_data.cFileName);
				if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!DeleteFilesInDir(file_path, file_names))
					{
						FindClose(file_handle);
						return false;
					}
					//RemoveDirectory(file_path); //clear empty subfolders
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
				else
				{
					if (file_names->find(file_data.cFileName) != file_names->end())
					{
						if (file_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
							SetFileAttributes(file_data.cFileName, file_data.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
						if (!DeleteFile(file_path))
						{  // delete the file traversed
							FindClose(file_handle);
							return false;
						}
					}
					wcscpy_s(file_path, ARRAYCOUNT(file_path), dir);
					wcscat_s(file_path, ARRAYCOUNT(file_path), pattern2);
				}
			}
			else
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
					bSearch = false;
				else
				{
					FindClose(file_handle);
					return false;
				}
			}
		}
		FindClose(file_handle);
		return true;
	}
	return false;
}

void FileIO::GetIniValue(wchar_t* field, wchar_t* key, wchar_t* value, unsigned long size)
{
#ifdef _DEBUG
	wchar_t ini_file_path[MAX_PATH];
	wcscpy_s(ini_file_path, ARRAYCOUNT(ini_file_path), (debug_exe_dir_w + L"\\config.ini").c_str());
	::GetPrivateProfileStringW(field, key, NULL, value, size, ini_file_path);
#else
	std::wstring dir;
	OS::CommonDirectory::GetExeDirW(&dir);
	dir += L"\\config.ini";
	::GetPrivateProfileStringW(field, key, NULL, value, size, dir.c_str());
#endif
}

void FileIO::GetIniValueA(char* field, char* key, char* value, unsigned long size)
{
#ifdef _DEBUG
	char ini_file_path[MAX_PATH];
	strcpy_s(ini_file_path, ARRAYCOUNT(ini_file_path), (debug_exe_dir + "\\config.ini").c_str());
	::GetPrivateProfileStringA(field, key, NULL, value, size, ini_file_path);
#else
	std::string dir;
	OS::CommonDirectory::GetExeDirA(&dir);
	dir += "\\config.ini";
	::GetPrivateProfileStringA(field, key, NULL, value, size, dir.c_str());
#endif
}

void FileIO::SetIniValue(wchar_t* field, wchar_t* key, wchar_t* value)
{
#ifdef _DEBUG
	wchar_t ini_file_path[MAX_PATH];
	wcscpy_s(ini_file_path, ARRAYCOUNT(ini_file_path), (debug_exe_dir_w + L"\\config.ini").c_str());
	::WritePrivateProfileStringW(field, key, value, ini_file_path);
#else
	std::wstring dir;
	OS::CommonDirectory::GetExeDirW(&dir);
	dir += L"\\config.ini";
	::WritePrivateProfileStringW(field, key, value, dir.c_str());
#endif
}

void FileIO::SetIniValueA(char* field, char* key, char* value)
{
#ifdef _DEBUG
	char ini_file_path[MAX_PATH];
	strcpy_s(ini_file_path, ARRAYCOUNT(ini_file_path), (debug_exe_dir + "\\config.ini").c_str());
	::WritePrivateProfileStringA(field, key, value, ini_file_path);
#else
	std::string dir;
	OS::CommonDirectory::GetExeDirA(&dir);
	dir += "\\config.ini";
	::WritePrivateProfileStringA(field, key, value, dir.c_str());
#endif
}

void FileIO::Text::ReplaceChar(char* file_path, char* new_file_path, char from, char to)
{
	if (from == to)
		return;

	FILE* fpr;
	FILE* fpw;
	fopen_s(&fpr, file_path, "r");
	fopen_s(&fpw, new_file_path, "w");
	if (fpr && fpw)
	{
		int ch;
		/* Note that the data type is int, not char */
		while ((ch = fgetc(fpr)) != EOF)
		{
			if (ch == from)
			{
				fputc(to, fpw);
			}
			else
			{
				fputc(ch, fpw);
			}
		}
	}
	fclose(fpw);
	fclose(fpr);
}

void FileIO::CSV::GetToken(std::string* token, char* line, int num)
{
	if (num <= 0)
		return;

	std::istringstream stream(line);
	while (getline(stream, *token, ','))
	{
		if ((--num) == 0)
			return;
	}

	*token = "";
	return;
} 