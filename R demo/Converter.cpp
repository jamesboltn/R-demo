#pragma once
#include"preinclude.h"
#include"Utilities.h"

void TypeConverter::WCharStrToCharStr(const wchar_t* wchar_str, char* char_str, size_t length)
{
	size_t i;
	wcstombs_s(&i, char_str, length, wchar_str, length);
}

void TypeConverter::CharStrToWCharStr(const char* char_str, wchar_t* wchar_str, int length)
{
	int len(MultiByteToWideChar(CP_ACP, 0, char_str, -1, NULL, 0));
	MultiByteToWideChar(CP_ACP, 0, char_str, -1, (LPWSTR)wchar_str, len);
}

void TypeConverter::StringToWString(std::wstring* str_w, const std::string* str_a)
{
	*str_w = L"";
	str_w->resize(str_a->length(), L'\0');
	std::transform(str_a->begin(), str_a->end(), str_w->begin(), TypeConverter::CharToWChar);
}

char TypeConverter::WCharToChar(wchar_t wc)
{
	// Simple type cast works because UNICODE incorporates ASCII into itself
	char c((char)wc);
	return c;
}

wchar_t TypeConverter::CharToWChar(char c)
{
	// Simple type cast works because UNICODE incorporates ASCII into itself
	wchar_t wc((wchar_t)c);
	return wc;
}

void TypeConverter::CharStrToWString(const char* char_str, std::wstring* wstr)
{
	int str_len = (int)strlen(char_str);
	if (str_len <= 0)
	{
		*wstr = L"";
		return;
	}
	
	int len = MultiByteToWideChar(CP_ACP, 0, char_str, str_len, 0, 0);
	*wstr = std::wstring((unsigned int)len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, char_str, str_len, &(*wstr)[0], len);
}

void TypeConverter::WCharStrToString(const wchar_t* wchar_str, std::string* str)
{
	int str_len = (int)wcslen(wchar_str);
	if (str_len <= 0)
	{
		*str = "";
		return;
	}

	int len = WideCharToMultiByte(CP_UTF8, 0, wchar_str, str_len, NULL, 0, NULL, NULL);
	*str = std::string((unsigned int)len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wchar_str, str_len, &(*str)[0], len, NULL, NULL);
}

void TypeConverter::WStringToString(std::string* str_a, const std::wstring* str_w)
{
	*str_a = "";
	str_a->resize(str_w->length(), '\0');

	std::transform(str_w->begin(), str_w->end(), str_a->begin(), WCharToChar);
}