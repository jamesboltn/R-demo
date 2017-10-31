#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class CheckBox :
		public UIBase,
		public ControlBase
	{
	public:
		LIB CheckBox(const wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit CheckBox(HWND _hwnd);//accepts HWND of created CheckBox
		LIB CheckBox() {}
		LIB virtual ~CheckBox() {}

		LIB bool Checked();
		LIB void Check();
		LIB void UnCheck();
	};

}
