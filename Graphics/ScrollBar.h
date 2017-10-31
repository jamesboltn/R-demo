#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class ScrollBar :
		public UIBase,
		public ControlBase
	{
	public:
		LIB ScrollBar(ScrollBarStyle style, int x_pos, int y_pos, int width, int height, const char* _unique_name = NULL);
		LIB explicit ScrollBar(HWND _hwnd);//accepts HWND of created ScrollBar
		LIB ScrollBar() {}
		LIB virtual ~ScrollBar() {}
	};
}
