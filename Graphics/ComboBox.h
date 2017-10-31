#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class ComboBox :
		public UIBase,
		public ControlBase
	{
	public:
		LIB ComboBox(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const const char* _unique_name = NULL);
		LIB ComboBox() {}
		LIB virtual ~ComboBox() {}
		LIB explicit ComboBox(HWND _hwnd);//accepts HWND of created ComboBox

		LIB void AddString(const wchar_t* item);
		LIB void AddStringA(const char* item);
		//index is zero based
		LIB void Remove(unsigned int index);
		LIB void Clear();
		LIB void ShowDropDown();
		LIB void SetSelection(unsigned int index);
		LIB int GetSelectIndex();
		LIB void GetSelectString(wchar_t* buffer, unsigned int length);
		LIB void GetSelectStringA(char* buffer, unsigned int length);
		LIB int GetDropDownWidth();
		LIB void SetDropDownWidth(unsigned int width);
		LIB bool IsListDroppedDown();
	};
}
