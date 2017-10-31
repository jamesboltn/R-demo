#pragma once
#include"DateTimePicker.h"

GuruLib::DateTimePicker::DateTimePicker(int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	window_class_name = DATETIMEPICK_CLASS;
	style_flag = WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHOWNONE;
	y = y_pos;
	x = x_pos;
	height = _height;
	width = _width;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}
GuruLib::DateTimePicker::DateTimePicker(HWND _hwnd)
{
	hwnd = _hwnd;
}