#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class Static :
		public UIBase,
		public ControlBase
	{
	private:
	public:
		LIB Static(const wchar_t* text, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit Static(HWND _hwnd);//accepts HWND of created Static
		LIB Static() {}
		LIB virtual ~Static() {}

		LIB virtual void SetText(const wchar_t* text);
	};

}

