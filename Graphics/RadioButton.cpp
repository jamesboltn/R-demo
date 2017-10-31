#pragma once
#include"RadioButton.h"

GuruLib::RadioButton::RadioButton(const wchar_t* caption, int x_pos, int y_pos, int _width, int _height, GuruLib::UnitType type, const char* _unique_name)
{
	window_name = caption;
	window_class_name = L"BUTTON";
	style_flag = WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON;
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

	//repaint button to fit its content
	SIZE size;
	if (Button_GetIdealSize(hwnd, &size) == TRUE)
		::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
}
GuruLib::RadioButton::RadioButton(HWND _hwnd)
{
	hwnd = _hwnd;
}
