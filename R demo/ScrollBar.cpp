#pragma once
#include"preinclude.h"
#include"ScrollBar.h"

ScrollBar::ScrollBar(ScrollBarStyle style, int x_pos, int y_pos, int width, int height, char* _unique_name)
{
	lpClassName = L"SCROLLBAR";

	dwStyle = WS_CHILD | WS_VISIBLE;
	if (style == HORIZONTAL)
	{
		dwStyle |= SBS_HORZ;
	}
	else if (style == VERTICAL)
	{
		dwStyle |= SBS_VERT;
	}
	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);
	_CreateWindow();

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}

ScrollBar::ScrollBar(HWND _hwnd)
{
	hwnd = _hwnd;
}
