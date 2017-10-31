#pragma once
#include"Static.h"

GuruLib::Static::Static(const wchar_t* text, int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	window_name = text;
	window_class_name = WC_STATIC;
	style_flag = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | SS_LEFT;
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

	//the following coommented code stretches the text to a single line width to fit its content, regardless of the original text occupying one line or multiple lines. 
	//HDC hdc = ::GetDC(hwnd);
	//SIZE size;
	//::GetTextExtentPoint32(hdc, text, (int)wcslen(text), &size);
	//::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
	//::ReleaseDC(hwnd, hdc);
}
GuruLib::Static::Static(HWND _hwnd)
{
	hwnd = _hwnd;
}
void GuruLib::Static::SetText(const wchar_t* text)
{
	Static_SetText(hwnd, text);
	HDC hdc(::GetDC(hwnd));

	SIZE size;
	::GetTextExtentPoint32(hdc, text, (int)wcslen(text), &size);
	::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);

	::ReleaseDC(hwnd, hdc);
}
