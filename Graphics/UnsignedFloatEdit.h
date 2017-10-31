#pragma once
#include"Edit.h"

namespace GuruLib
{
	class UnsignedFloatEdit :
		public Edit
	{
	private:
	public:
		UnsignedFloatEdit() {}
		UnsignedFloatEdit(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
		explicit UnsignedFloatEdit(HWND _hwnd);//accepts HWND of created UnsignedFloatEdit
	};
}

