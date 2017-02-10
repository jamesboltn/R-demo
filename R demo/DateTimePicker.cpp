#pragma once
#include"preinclude.h"
#include"DateTimePicker.h"

DateTimePicker::DateTimePicker(int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpClassName = DATETIMEPICK_CLASS;
	dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHOWNONE;
	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}

DateTimePicker::DateTimePicker(HWND _hwnd)
{
	hwnd = _hwnd;
}