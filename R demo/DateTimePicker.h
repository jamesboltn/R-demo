#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class DateTimePicker :
	public UIBase,
	public ControlBase
{
public:
	DateTimePicker(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	DateTimePicker(HWND _hwnd);//accepts HWND of created DateTimePicker
	DateTimePicker() {}
	virtual ~DateTimePicker() {}
};
