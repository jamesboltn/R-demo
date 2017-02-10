#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class ListBox :
	public UIBase,
	public ControlBase
{
public:
	ListBox(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	ListBox(HWND _hwnd);//accepts HWND of created ListBox
	ListBox() {}
	virtual ~ListBox() {}
	void AddString(std::vector<wchar_t*>* items);
	void AddString(wchar_t* str);
	void AddStringA(char* str);
	int GetSelectIndex();
};
