#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class ComboBox :
	public UIBase,
	public ControlBase
{
public:
	ComboBox(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	ComboBox() {}
	virtual ~ComboBox() {}
	ComboBox(HWND _hwnd);//accepts HWND of created ComboBox
	void AddString(wchar_t* item);
	void AddStringA(char* item);
	//index is zero based
	void Remove(unsigned int index);
	void Clear();
	void ShowDropDown();
	void SetSelection(unsigned int index);
	int GetSelectIndex();
	void GetSelectString(wchar_t* buffer, unsigned int length);
	void GetSelectStringA(char* buffer, unsigned int length);
	int GetDropDownWidth();
	void SetDropDownWidth(unsigned int width);
	bool IsListDroppedDown();
};
