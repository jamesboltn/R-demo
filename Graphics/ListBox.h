#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class ListBox :
		public UIBase,
		public ControlBase
	{
	public:
		ListBox(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		explicit ListBox(HWND _hwnd);//accepts HWND of created ListBox
		ListBox() {}
		LIB virtual ~ListBox() {}

		LIB void AddString(const std::vector<wchar_t*> &items);
		LIB void AddString(const wchar_t* str);
		LIB void AddStringA(const char* str);
		LIB int GetSelectIndex();
	};

}
