#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace TypeConverter
	{
		LIB void WCharStrToCharStr(const wchar_t* wchar_str, char* char_str, size_t length);

		LIB void CharStrToWCharStr(const char* char_str, wchar_t* wchar_str, int length);

		LIB void CharStrToWString(const char* char_str, std::wstring* wstr);

		LIB void WCharStrToString(const wchar_t* wchar_str, std::string* str);

		LIB char WCharToChar(wchar_t wc);

		LIB wchar_t CharToWChar(char c);

		//only for strings with ANSI characters
		LIB void StringToWString(const std::string &str_a, std::wstring &str_w);

		//only for wide strings with ANSI characters
		LIB void WStringToString(const std::wstring &str_w, std::string &str_a);
	}

	//typedef std::wstring EventName;
	//typedef int WaitCount;
	//class Event
	//{
	//private:
	//	std::wstring event_name;
	//	static std::unordered_map<EventName, WaitCount> event_names;
	//public:
	//	Event() :
	//		event_name(L"")
	//	{
	//
	//	}
	//	Event(std::wstring event_name, int wait_count = 1);
	//	bool Wait(unsigned long milliseconds = INFINITE);
	//	static bool Wait(std::wstring event_name, unsigned long milliseconds = INFINITE);
	//	static void Signal(std::wstring event_name);
	//};

	////PURE_STATIC namespace Container extend features to various STL containers
	//PURE_STATIC namespace Container
	//{
	//	PURE_STATIC namespace Vector
	//	{
	//		//if typename T is set with a custom class, then this class must have operator< defined for std::sort and operator== defined for std::unique
	//		template<typename T>
	//		void RemoveDuplicate(std::vector<T>* vec);
	//
	//		template<typename T>
	//		bool IsExist(std::vector<T>* vec, T* element);
	//
	//		template<typename T>
	//		void Clear(std::vector<T>* vec);
	//
	//		//template<typename T>
	//		//APICALL void Erase(std::vector<T> &vec);
	//	}
	//
	//	PURE_STATIC namespace Stack
	//	{
	//		template<typename T>
	//		void Clear(std::stack<T>* stk);
	//	}
	//
	//	PURE_STATIC namespace Map
	//	{
	//		//return -1 means element does not exist
	//		template<typename Key, typename Value>
	//		int GetIndexByKey(std::unordered_map<Key, Value>* map, Key* key);
	//	}
	//}
	//
	//class Mutex
	//{
	//public:
	//	Mutex(wchar_t* name);
	//	virtual ~Mutex();
	//private:
	//	HANDLE mutex;
	//};
	//
	//template<typename T>
	//class Singleton
	//{
	//private:
	//	static T* instance = NULL;
	//	Singleton()
	//	{
	//		//private constructor
	//	}
	//protected:
	//
	//public:
	//	static T* GetInstance()
	//	{
	//		if (!intance)
	//		{
	//			instance = new T();
	//		}
	//		return instance;
	//	}
	//	static void ReleaseInstance()
	//	{
	//		if (intance)
	//		{
	//			delete instance;
	//          instance = NULL;
	//		}
	//	}
	//};
	//
	////this base class frees you from writing comparison operator overload functions again
	//template<typename T>
	//class Compare
	//{
	//private:
	//	virtual double compare(const T &other) = 0;
	//protected:
	//
	//public:
	//	Compare() {}
	//	virtual ~Compare() {}
	//
	//	//all the comparison operators
	//	bool operator <(const T &other) { return compare(other) < 0; }
	//	bool operator <=(const T &other) { return compare(other) <= 0; }
	//	bool operator ==(const T &other) { return compare(other) == 0; }
	//	bool operator !=(const T &other) { return compare(other) != 0; }
	//	bool operator >=(const T &other) { return compare(other) >= 0; }
	//	bool operator >(const T &other) { return compare(other) > 0; }
	//};
	//
	//PURE_STATIC namespace SmartPointer
	//{
	//	template<typename T>
	//	class AutoPointer
	//	{
	//	private:
	//		T* raw_pointer;
	//	protected:
	//
	//	public:
	//		//constructor
	//		explicit AutoPointer(T* ptr = NULL) throw()
	//		{
	//			if (ptr)
	//			{
	//				raw_pointer = ptr;
	//			}
	//			else
	//			{
	//				raw_pointer = NULL;
	//			}
	//		}
	//		//copy constructor
	//		AutoPointer(const AutoPointer<T> &auto_ptr_ref) throw()
	//		{
	//			if (auto_ptr_ref)
	//			{
	//				raw_pointer = auto_ptr_ref;
	//			}
	//			else
	//			{
	//				raw_pointer = NULL;
	//			}
	//		}
	//		//assignment operator overload
	//		AutoPointer<T>& operator=(AutoPointer<T> &auto_ptr_ref) throw()
	//		{
	//			if (auto_ptr_ref)
	//			{
	//				raw_pointer = auto_ptr_ref;
	//			}
	//			else
	//			{
	//				raw_pointer = NULL;
	//			}
	//			return raw_pointer;
	//		}
	//		//destructor
	//		~AutoPointer()
	//		{
	//			if (raw_pointer)
	//			{
	//				delete raw_pointer;
	//				raw_pointer = NULL;
	//			}
	//		}
	//		//* operator overload
	//		T& operator*() const throw()
	//		{
	//			return *raw_pointer;
	//		}
	//		//-> operator overload
	//		T* operator->() const throw()
	//		{
	//			return raw_pointer;
	//		}
	//		//function to get pointer to the heap object
	//		T* get() const throw()
	//		{
	//			return raw_pointer;
	//		}
	//	};
	//}

}
