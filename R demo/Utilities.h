#pragma once
#include"preinclude.h"

namespace FileIO
{
	void GetIniValue(wchar_t* field, wchar_t* key, wchar_t* value, unsigned long size);
	void GetIniValueA(char* field, char* key, char* value, unsigned long size);
	void SetIniValue(wchar_t* field, wchar_t* key, wchar_t* value);
	void SetIniValueA(char* field, char* key, char* value);
	/************************set and write*******************************/
	bool DirectoryExists(wchar_t* dir);//determines if a given directory exists
	bool ClearDirectory(const wchar_t* dir, const wchar_t* dir_same_with_prior_param);//clear the content in a given directory while reserving it
	bool ClearDirectory(wchar_t* dir);//clear the content in a given directory and delete itself

	//delete files located in a given directory or its subdirectory
	bool DeleteFilesInDir(wchar_t* dir, std::unordered_set<wchar_t*>* file_names);

	void WriteLog(const char* msg, FILE* fp, std::string* time_stamp = NULL);
	//TODO:remove log"succeeded in logging in quote order server use appendline instead"
	void AppendText(FILE* fp, char* msg);

	namespace Text
	{
		//this function will replace all specific chars file with a new char, generate a new file 
		void ReplaceChar(char* file_path, char* new_file_path, char from, char to);
	}

	namespace CSV
	{
		void GetToken(std::string* token, char* line, int num);
	}
}
namespace TypeConverter
{
	void WCharStrToCharStr(const wchar_t* wchar_str, char* char_str, size_t length);

	void CharStrToWCharStr(const char* char_str, wchar_t* wchar_str, int length);

	void CharStrToWString(const char* char_str, std::wstring* wstr);

	void WCharStrToString(const wchar_t* wchar_str, std::string* str);

	char WCharToChar(wchar_t wc);

	wchar_t CharToWChar(char c);

	//only for strings with ANSI characters
	void StringToWString(std::wstring* str_w, const std::string* str_a);

	//only for wide strings with ANSI characters
	void WStringToString(std::string* str_a, const std::wstring* str_w);
}
namespace String
{
	void ReplaceAll(std::string* str, std::string* from, std::string* to);

	bool EndWith(std::string* full, std::string* ending);

	void Split(std::string* str, char delimiter, std::vector<std::string>* tokens);

	void RemoveAllChars(std::string* str, char c);

	void RemoveLastChar(std::string* str);

	void RemoveLastCharOnCondition(std::string* str, char c);

	void RemoveLastOccurrenceOfChar(std::string* str, char c);

	void RemoveFirstCharOccurrence(std::string* str, char c);

	void RemoveFirstSubstring(std::string* str, std::string* sub_str);

	template<typename T>
	void RemoveAllSubstrings(std::basic_string<T>* str, std::basic_string<T>* sub_str);

	bool Contains(const std::string &str, const std::string &sub_str);

	namespace CharStr
	{
		void RemoveFirstChar(char* &str);

		void RemoveLastChar(char* &str);

		void RemoveAllChars(char* &str, char c);
	}
}
namespace WString
{
	bool Contains(std::wstring* str, std::wstring* sub_str);
}
namespace DateTime
{
	int GetCurrentYearInt();
	std::string GetCurrentYearString();
	int GetCurrentMonthInt();
	std::string GetCurrentMonthString();
	int GetCurrentDateInt();
	std::string GetCurrentDateString();
	std::string GetYesterDateString();
	std::string GetCurrentDayString();
	std::string GetCurrentTimeString();
	std::string GetCurrentDateTimeString();
	std::string GetSpecificDateTimeString(time_t* ptr_timet);	
	time_t GetUnixElapseFromString(std::string* time_str);
}
namespace MultiThread
{
	class CriticalSection
	{
	public:
		CriticalSection();
		void Lock();
		void UnLock();
		virtual ~CriticalSection();
	private:
		CRITICAL_SECTION m_stCriticalSection;
	};

	class AutoCriticalSection
	{
	public:
		AutoCriticalSection();
		virtual ~AutoCriticalSection();
	private:

	};
}

typedef std::wstring EventName;
typedef int WaitCount;
class Event
{
private:
	std::wstring event_name;
	static std::unordered_map<EventName, WaitCount> event_names;
public:
	Event():
		event_name(L"") 
	{

	}
	Event(std::wstring event_name, int wait_count = 1);
	bool Wait(unsigned long milliseconds = INFINITE);
	static bool Wait(std::wstring event_name, unsigned long milliseconds = INFINITE);
	static void Signal(std::wstring event_name);
};

namespace MachineInfo
{
	inline int GetScreenWidth();
	inline int GetScreenHeight();
}
namespace OS
{
	bool Reboot(int time_out = 0);
	bool StopShutDown();
	bool ShutDown(int time_out = 0);
	bool IsAlreadyRunning();
	/*APICALL bool BootOnStartUp(wchar_t* boot_name);*/
	//param 'boot_name' of OS::BootOnStartUp and OS::RemoveBootOnStartUp should be the same for the same application
	/*APICALL bool RemoveBootOnStartUp(wchar_t* boot_name);*/

	namespace CommonDirectory
	{
		std::wstring GetProgramFilesDirectoryW();
		std::wstring GetWindowsDirectoryW();
		std::wstring GetSystemDirectoryW();
		std::wstring GetMyDocumentsDirectoryW();
		std::wstring GetAppDataDirectoryW();
		std::wstring GetLocalAppDataDirectoryW();
		std::wstring GetDesktopDirectoryW();
		std::wstring GetStartupDirectoryW();
		void GetExeDirW(std::wstring* dir);
		void GetExeDirA(std::string* dir);
	}

	class InstanceLimit
	{
		int instance_count;
		std::vector<std::wstring> names;
		int current_instance_index;//one_based
		HANDLE this_handle;
	public:
		InstanceLimit(int limit = 1, wchar_t* name = NULL);
		virtual ~InstanceLimit();

		enum { MAX_INSTANCES = 16 };
		bool IsExceedLimit();
		inline int GetCurrentInstanceIndex() { return current_instance_index; }
	};
}

//namespace Container add additional features to various STL containers
namespace Container
{
	namespace Vector
	{
		//if typename T is set with a custom class, then this class must have operator< defined for std::sort and operator== defined for std::unique
		template<typename T>
		void RemoveDuplicate(std::vector<T>* vec);

		template<typename T>
		bool IsExist(std::vector<T>* vec, T* element);

		template<typename T>
		void Clear(std::vector<T>* vec);

		//template<typename T>
		//APICALL void Erase(std::vector<T> &vec);
	}

	namespace Stack
	{
		template<typename T>
		void Clear(std::stack<T>* stk);
	}

	namespace Map
	{
		//return -1 means element does not exist
		template<typename Key, typename Value>
		int GetIndexByKey(std::unordered_map<Key, Value>* map, Key* key);
	}
}

class Mutex
{
public:
	Mutex(wchar_t* name);
	virtual ~Mutex();
private:
	HANDLE mutex;
};
