#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class RadioButton :
	public UIBase,
	public ControlBase
{
public:
	RadioButton(wchar_t* window_caption, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	RadioButton(HWND _hwnd);//accepts HWND of created RadioButton
	RadioButton() {}
	virtual ~RadioButton() {}
};
