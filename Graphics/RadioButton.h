#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class RadioButton :
		public UIBase,
		public ControlBase
	{
	public:
		LIB RadioButton(const wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit RadioButton(HWND _hwnd);//accepts HWND of created RadioButton
		LIB RadioButton() {}
		LIB virtual ~RadioButton() {}
	};
}

