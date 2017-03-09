#pragma once
#include"preinclude.h"
#include"Utilities.h"

void String::ReplaceAll(std::string *str, std::string *from, std::string *to)
{
	if (from->empty())
		return;
	size_t start_pos(0);
	while ((start_pos = str->find(*from, start_pos)) != std::string::npos) {
		str->replace(start_pos, from->length(), *to);
		start_pos += to->length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

bool String::EndWith(std::string* full, std::string* ending)
{
	if (full->length() >= ending->length()) {
		return (0 == full->compare(full->length() - ending->length(), ending->length(), *ending));
	}
	else {
		return false;
	}
}

void String::Split(std::string* str, char delimiter, std::vector<std::string>* tokens)
{
	tokens->clear();
	std::istringstream stream(*str);
	std::string token;
	while (getline(stream, token, delimiter)) {
		tokens->push_back(token);
	}
}

void String::RemoveAllChars(std::string* str, char c)
{
	str->erase(std::remove(str->begin(), str->end(), c), str->end());
}

void String::RemoveFirstCharOccurrence(std::string* str, char c)
{
	auto pos(str->find(c));
	if (pos != std::string::npos) str->erase(pos, 1);
}

void String::RemoveFirstSubstring(std::string* str, std::string* sub_str)
{
	std::string::size_type i(str->find(*sub_str));

	if (i != std::string::npos)
		str->erase(i, sub_str->length());
}

template<typename T>
void String::RemoveAllSubstrings(std::basic_string<T>* str, std::basic_string<T>* sub_str)
{
	basic_string<T>::size_type n(sub_str->length());
	for (basic_string<T>::size_type i = str->find(sub_str); i != basic_string<T>::npos; i = str->find(sub_str))
		str->erase(i, n);
}

bool String::Contains(const std::string &str, const std::string &sub_str)
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

void String::CharStr::RemoveFirstChar(char* &str)
{
	++str;
}

void String::CharStr::RemoveLastChar(char* &str)
{
	str[strlen(str) - 1] = 0;
}

void String::CharStr::RemoveAllChars(char* &str, char c)
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

void String::RemoveLastChar(std::string* str)
{
	if (!str->empty())
	{

		str->pop_back();
	}

}

void String::RemoveLastCharOnCondition(std::string* str, char c)
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

void String::RemoveLastOccurrenceOfChar(std::string* str, char c)
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

bool WString::Contains(std::wstring* str, std::wstring* sub_str)
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