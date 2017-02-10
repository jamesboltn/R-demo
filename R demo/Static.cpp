#pragma once
#include"preinclude.h"
#include"Static.h"

Static::Static(wchar_t* text, int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpWindowName = text;
	lpClassName = WC_STATIC;
	dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL;
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

	HDC hdc(::GetDC(hwnd));

	SIZE size;
	::GetTextExtentPoint32(hdc, text, (int)wcslen(text), &size);
	::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);

	::ReleaseDC(hwnd, hdc);
}

void Static::SetText(const wchar_t* text)
{
	Static_SetText(hwnd, text);
	HDC hdc(::GetDC(hwnd));

	SIZE size;
	::GetTextExtentPoint32(hdc, text, (int)wcslen(text), &size);
	::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);

	::ReleaseDC(hwnd, hdc);
}

Static::Static(HWND _hwnd)
{
	hwnd = _hwnd;
}