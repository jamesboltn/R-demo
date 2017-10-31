#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class DateTimePicker :
		public UIBase,
		public ControlBase
	{
	public:
		LIB DateTimePicker(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit DateTimePicker(HWND _hwnd);//accepts HWND of created DateTimePicker
		LIB DateTimePicker() {}
		LIB virtual ~DateTimePicker() {}
	};
}

