#pragma once
#include"preinclude.h"
#include"Utilities.h"

Mutex::Mutex(wchar_t* name) :
	mutex(NULL)
{
	mutex = ::CreateMutex(NULL, FALSE, name);
	::WaitForSingleObject(mutex, INFINITE);
}

Mutex::~Mutex()
{
	if (mutex)
	{
		::ReleaseMutex(mutex);
	}
}
