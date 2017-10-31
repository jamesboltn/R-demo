#pragma once
#include"Synchronization.h"

GuruLib::Synchronization::CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&m_stCriticalSection);
}

void GuruLib::Synchronization::CriticalSection::Lock()
{
	::EnterCriticalSection(&m_stCriticalSection);
}

void GuruLib::Synchronization::CriticalSection::UnLock()
{
	::LeaveCriticalSection(&m_stCriticalSection);
}

GuruLib::Synchronization::CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&m_stCriticalSection);
}

GuruLib::Synchronization::AutoCriticalSection::AutoCriticalSection(CriticalSection* _critical_section):
	critical_section(_critical_section)
{
	critical_section->Lock();
}

GuruLib::Synchronization::AutoCriticalSection::~AutoCriticalSection()
{
	critical_section->UnLock();
}