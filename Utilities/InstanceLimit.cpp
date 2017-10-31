#pragma once
#include"InstanceLimit.h"

GuruLib::InstanceLimit::InstanceLimit(int limit /*= 1*/, wchar_t* name /*= NULL*/)
{
	assert(limit <= MAX_INSTANCES);
	instance_count = limit;
	std::wstring str(L"InstanceLimit");
	if (name)
		str = name;
	for (unsigned int j = 0; j < instance_count; ++j)
	{
		str += L"_";
		wchar_t buf[20];
		_itow_s(j, buf, GuruLib::ArraySize(buf), 10);
		str += buf;
		names.push_back(str);
	}
	this_handle = NULL;
	current_instance_index = 0;
}

GuruLib::InstanceLimit::~InstanceLimit()
{
	::CloseHandle(this_handle);
}

bool GuruLib::InstanceLimit::ExceedLimit()
{
	bool exceed_limit = true;
	HANDLE h = NULL;
	for (unsigned int j = 0; j < instance_count; ++j)
	{
#pragma warning(push)
#pragma warning(disable:4365)
		h = ::CreateMutex(NULL, FALSE, names[j].c_str());
#pragma warning(pop)
		assert(h);
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			::CloseHandle(h);
		}
		else
		{
			current_instance_index = j + 1;
			this_handle = h;
			exceed_limit = false;
			break;
		}
	}
	return exceed_limit;
}