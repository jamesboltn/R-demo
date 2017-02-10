#pragma once
#include"preinclude.h"
#include"RadioButton.h"

RadioButton::RadioButton(wchar_t* caption, int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpWindowName = caption;
	lpClassName = L"BUTTON";
	dwStyle = WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON;
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

	//repaint button to fit its content
	SIZE size;
	if (Button_GetIdealSize(hwnd, &size) == TRUE)
		::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
}

RadioButton::RadioButton(HWND _hwnd)
{
	hwnd = _hwnd;
}
