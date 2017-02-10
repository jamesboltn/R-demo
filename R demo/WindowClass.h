#pragma once
#include"preinclude.h"

class WindowClass :
	public WNDCLASSEX
{
public:
	const wchar_t* messagebox_title;
	WindowClass();
};
