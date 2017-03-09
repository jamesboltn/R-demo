#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class GroupBox :
	public UIBase,
	public ControlBase
{
public:
	GroupBox(wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	GroupBox(HWND _hwnd);//accepts HWND of created GroupBox
	GroupBox() {}
	virtual ~GroupBox() {}
};