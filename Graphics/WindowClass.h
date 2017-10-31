#pragma once
#include"PreInclude.h"

namespace GuruLib
{
	class WindowClass :
		public WNDCLASSEX
	{
	public:
		const wchar_t* messagebox_title;

		WindowClass();
	};
}
