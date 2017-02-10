#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class CheckBox :
	public UIBase,
	public ControlBase
{
public:
	CheckBox(wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	CheckBox(HWND _hwnd);//accepts HWND of created CheckBox
	CheckBox() {}
	virtual ~CheckBox() {}
};
