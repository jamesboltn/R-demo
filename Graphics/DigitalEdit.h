#pragma once
#include"Edit.h"

namespace GuruLib
{
	class DigitalEdit :
		public Edit
	{
	public:
		DigitalEdit() {}
		DigitalEdit(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
		explicit DigitalEdit(HWND _hwnd);//accepts HWND of created DigitalEdit
	};
}

