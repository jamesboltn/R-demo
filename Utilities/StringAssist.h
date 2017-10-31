#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace StdString
	{
		LIB void ReplaceAll(std::string *str, std::string *from, std::string *to);
		LIB bool EndWith(const std::string &full, const std::string &ending_sub);
		LIB bool StartWith(const std::string &full, const std::string &start_sub);
		LIB void Split(std::string &str, char delimiter, std::vector<std::string> &tokens);
		LIB void RemoveAllChars(std::string* str, char c);
		LIB void RemoveFirstCharOccurrence(std::string* str, char c);
		LIB void RemoveFirstSubstring(std::string* str, std::string* sub_str);
		//template<typename T>
		//LIB void RemoveAllSubstrings(std::basic_string<T>* str, std::basic_string<T>* sub_str);
		LIB bool Contains(const std::string &str, const std::string &sub_str);
		LIB std::string CreateUpperCaseCopy(const std::string &src);
		LIB void RemoveLastChar(std::string* str);
		LIB void RemoveLastCharOnCondition(std::string* str, char c);
		LIB void RemoveLastOccurrenceOfChar(std::string* str, char c);
	}
	
	namespace CStyleString
	{
		LIB void RemoveFirstChar(char* &str);
		LIB void RemoveLastChar(char* &str);
		LIB void RemoveAllChars(char* &str, char c);
		LIB bool StartsWith(const char* str, const char* prefix);
	}

	namespace StdWString
	{
		LIB bool Contains(std::wstring* str, std::wstring* sub_str);
		LIB std::wstring CreateUpperCaseCopy(const std::wstring &src);
	}
}


