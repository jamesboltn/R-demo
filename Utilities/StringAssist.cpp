#pragma once
#include"StringAssist.h"

void GuruLib::StdString::ReplaceAll(std::string *str, std::string *from, std::string *to)
{
	if (from->empty())
		return;
	size_t start_pos(0);
	while ((start_pos = str->find(*from, start_pos)) != std::string::npos) {
		str->replace(start_pos, from->length(), *to);
		start_pos += to->length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

bool GuruLib::StdString::EndWith(const std::string &full, const std::string &ending_sub)
{
	if (full.length() >= ending_sub.length()) {
		return (0 == full.compare(full.length() - ending_sub.length(), ending_sub.length(), ending_sub));
	}
	else {
		return false;
	}
}

bool GuruLib::StdString::StartWith(const std::string &full, const std::string &start_sub)
{
	if (full.length() >= start_sub.length()) {
		return (0 == full.compare(0, start_sub.length(), start_sub));
	}
	else {
		return false;
	}
}

void GuruLib::StdString::Split(std::string &str, char delimiter, std::vector<std::string> &tokens)
{
	tokens.clear();
	std::istringstream stream(str);
	std::string token;
	while (getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}
}

void GuruLib::StdString::RemoveAllChars(std::string* str, char c)
{
	str->erase(std::remove(str->begin(), str->end(), c), str->end());
}

void GuruLib::StdString::RemoveFirstCharOccurrence(std::string* str, char c)
{
	auto pos(str->find(c));
	if (pos != std::string::npos) str->erase(pos, 1);
}

void GuruLib::StdString::RemoveFirstSubstring(std::string* str, std::string* sub_str)
{
	std::string::size_type i(str->find(*sub_str));

	if (i != std::string::npos)
		str->erase(i, sub_str->length());
}

//template<typename T>
//void String::RemoveAllSubstrings(std::basic_string<T>* str, std::basic_string<T>* sub_str)
//{
//	basic_string<T>::size_type n(sub_str->length());
//	for (basic_string<T>::size_type i = str->find(sub_str); i != basic_string<T>::npos; i = str->find(sub_str))
//		str->erase(i, n);
//}

bool GuruLib::StdString::Contains(const std::string &str, const std::string &sub_str)
{
	if (str.find(sub_str) != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GuruLib::CStyleString::RemoveFirstChar(char* &str)
{
	++str;
}

void GuruLib::CStyleString::RemoveLastChar(char* &str)
{
	str[strlen(str) - 1] = 0;
}

void GuruLib::CStyleString::RemoveAllChars(char* &str, char c)
{
	/*for (unsigned int i = 0; i < strlen(str); ++i)
	if (str[i] == c)
	strcpy(str + i, str + i + 1);*/

	unsigned i, j; /* need to declare at top for 'C' compile ... */

	for (i = j = 0; str[i] != 0; ++i)
		if (str[i] == c) continue;
		else str[j++] = str[i];

		str[j] = 0; /* Now ... null terminate */
}

void GuruLib::StdString::RemoveLastChar(std::string* str)
{
	if (!str->empty())
	{

		str->pop_back();
	}

}

void GuruLib::StdString::RemoveLastCharOnCondition(std::string* str, char c)
{
	if (!str->empty())
	{
		auto it(str->end() - 1);
		if (*it == c)
		{
			str->pop_back();
		}
	}
}

void GuruLib::StdString::RemoveLastOccurrenceOfChar(std::string* str, char c)
{
	if (!str->empty())
	{
		std::string::size_type pos(str->find_last_of(c));
		if (pos != std::string::npos)
		{
			str->erase(pos);
		}
	}

}

bool GuruLib::StdWString::Contains(std::wstring* str, std::wstring* sub_str)
{
	if (str->find(*sub_str) != std::wstring::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GuruLib::CStyleString::StartsWith(const char* str, const char* prefix)
{
	size_t lenpre = strlen(prefix),
		lenstr = strlen(str);
	return lenstr < lenpre ? false : strncmp(prefix, str, lenpre) == 0;
}

std::string GuruLib::StdString::CreateUpperCaseCopy(const std::string &src)
{
	if (src.empty())
	{
		return src;
	}

	std::vector<char> buffer(src.begin(), src.end());
	buffer.push_back(0);

	::CharUpperBuffA(&buffer.at(0), static_cast<DWORD>(src.size()));

	return (&buffer.front());
}

std::wstring GuruLib::StdWString::CreateUpperCaseCopy(const std::wstring &src)
{
	if (src.empty()) return src;

	std::vector<wchar_t> buffer(src.begin(), src.end());
	buffer.push_back(0);

	::CharUpperBuffW(&buffer.at(0), static_cast<DWORD>(src.size()));

	return (&buffer.front());
}