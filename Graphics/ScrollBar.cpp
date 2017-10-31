#pragma once
#include"ScrollBar.h"

GuruLib::ScrollBar::ScrollBar(ScrollBarStyle style, int x_pos, int y_pos, int _width, int _height, const char* _unique_name)
{
	window_class_name = L"SCROLLBAR";

	style_flag = WS_CHILD | WS_VISIBLE;
	if (style == HORIZONTAL)
	{
		style_flag |= SBS_HORZ;
	}
	else if (style == VERTICAL)
	{
		style_flag |= SBS_VERT;
	}
	y = y_pos;
	x = x_pos;
	height = _height;
	width = _width;
	hMenu = HMENU(resource_id);
	_CreateWindow();

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}
GuruLib::ScrollBar::ScrollBar(HWND _hwnd)
{
	hwnd = _hwnd;
}
