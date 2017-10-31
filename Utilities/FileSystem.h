#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace FileSystem
	{
		class File
		{
		public:
			static void WriteLog(const char* msg, FILE* fp, std::string* time_stamp = NULL);
			static void GetFileMd5Hash(const char* file_path, OUT std::string &buffer);
			static bool GetFileVersionString(const wchar_t* file_path, OUT std::wstring &version_string);//file path can be a relatively file path or a fully qualified one
			static bool HasVersionString(const wchar_t* file_path);//Check if the specified file has been versioned. File path can be a relative path or a fully qualified one.
		};

		namespace Directory
		{
			LIB BOOL Delete(const wchar_t* dir);//remove an entire tree in a given directory and delete itself. this method can fail because a process has a folder or subfolder as its current directory
			LIB BOOL Copy(const wchar_t* original_dir, wchar_t* destination_dir);//usage Copy(L"D:\\SourceDir\\", L"D:\\DestDir\\")
			namespace Common
			{
				LIB std::wstring GetMyDocumentsDirectoryW();
				LIB std::wstring GetAppDataDirectoryW();
				LIB std::wstring GetLocalAppDataDirectoryW();
				LIB std::wstring GetDesktopDirectoryW();
				LIB std::wstring GetStartupDirectoryW();
				LIB void GetExeDirW(std::wstring* dir);
				LIB void GetExeDirA(std::string* dir);
			}
		}
		//	/************************set and write*******************************/
		//	bool DirectoryExists(wchar_t* dir);//determines if a given directory exists
		//	bool ClearDirectory(const wchar_t* dir, const wchar_t* dir_same_with_prior_param);//clear the content in a given directory while reserving it
		//	bool ClearDirectory(wchar_t* dir);//clear the content in a given directory and delete itself
		//
		//									  //delete files located in a given directory or its subdirectory
		//	bool DeleteFilesInDir(wchar_t* dir, std::unordered_set<wchar_t*>* file_names);
		//
		//	//TODO:remove log"succeeded in logging in quote order server use appendline instead"
		//	void AppendText(FILE* fp, char* msg);
		//
		//	PURE_STATIC namespace Text
		//	{
		//		//this function will replace all specific chars file with a new char, generate a new file 
		//		void ReplaceChar(char* file_path, char* new_file_path, char from, char to);
		//	}
		//
		//	PURE_STATIC namespace CSV
		//	{
		//		void GetToken(std::string* token, char* line, int num);
		//	}
	}
}

