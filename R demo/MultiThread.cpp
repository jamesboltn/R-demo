#pragma once
#include"preinclude.h"
#include"Utilities.h"

#pragma region critical section
MultiThread::CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&m_stCriticalSection);
}

void MultiThread::CriticalSection::Lock()
{
	EnterCriticalSection(&m_stCriticalSection);
}

void MultiThread::CriticalSection::UnLock()
{
	LeaveCriticalSection(&m_stCriticalSection);
}

MultiThread::CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_stCriticalSection);
}

static MultiThread::CriticalSection critical_section;

MultiThread::AutoCriticalSection::AutoCriticalSection()
{
	critical_section.Lock();
}

MultiThread::AutoCriticalSection::~AutoCriticalSection()
{
	critical_section.UnLock();
}
#pragma endregion

#pragma region event

std::unordered_map<EventName, WaitCount> Event::event_names({ 0 });

Event::Event(std::wstring _event_name, int wait_count)
{
	if (wait_count >= 1 &&event_names.find(_event_name) == event_names.end())
	{
		::CreateEvent(NULL, false, false, _event_name.c_str());
		event_names.insert(std::make_pair(_event_name, wait_count));
		event_name = _event_name;
	}
	else
	{
		event_name = L"";
	}
}

bool Event::Wait(std::wstring _event_name, unsigned long milliseconds)
{
	auto it(event_names.find(_event_name));
	if (it != event_names.end())
	{
		HANDLE event = ::OpenEvent(EVENT_ALL_ACCESS, false, _event_name.c_str());
		if (event)
		{
			::WaitForSingleObject(event, milliseconds);
			if (--event_names[_event_name] == 0)
			{
				event_names.erase(it);
				::CloseHandle(event);
			}
			return true;
		}
		return false;
	}
	return false;
}

bool Event::Wait(unsigned long milliseconds)
{
	if(event_name != L"")
	{
		auto it(event_names.find(event_name));
		if (it != event_names.end())
		{
			HANDLE event = ::OpenEvent(EVENT_ALL_ACCESS, false, event_name.c_str());
			if (event)
			{
				::WaitForSingleObject(event, milliseconds);
				if (--event_names[event_name] == 0)
				{
					event_names.erase(it);
					::CloseHandle(event);
				}
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

void Event::Signal(std::wstring _event_name)
{
	auto it(event_names.find(_event_name));
	if (it != event_names.end())
	{
		HANDLE event = ::OpenEvent(EVENT_ALL_ACCESS, false, _event_name.c_str());
		if (event)
		{
			::SetEvent(event);
		}
	}
}
#pragma endregion