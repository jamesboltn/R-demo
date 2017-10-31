#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class GroupBox :
		public UIBase,
		public ControlBase
	{
	public:
		LIB GroupBox(const wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit GroupBox(HWND _hwnd);//accepts HWND of created GroupBox
		LIB GroupBox() {}
		LIB virtual ~GroupBox() {}
	};
}
