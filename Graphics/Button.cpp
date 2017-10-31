#pragma once
#include"Button.h"
#include"Window.h"

GuruLib::Button::Button(const wchar_t* button_text, int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	// --- we're going to assume the following members are declared in Window as protected
	window_name = button_text;
	window_class_name = L"BUTTON";
	style_flag = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
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
GuruLib::Button::Button(const wchar_t* caption, const RECT &rect, const char* _unique_name)
{
	window_name = caption;
	window_class_name = L"BUTTON";
	style_flag = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	y = rect.top;
	x = rect.left;
	height = rect.bottom - rect.top;
	width = rect.right - rect.left;
	hMenu = HMENU(resource_id); 
	_CreateWindow();

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}

	//repaint button to fit its content
	SIZE size;
	if (Button_GetIdealSize(hwnd, &size) == TRUE)
		::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
}
void GuruLib::Button::SetText (const wchar_t* button_text)
{
	::Button_SetText(hwnd, button_text);

	//repaint button to fit its content
	SIZE size;
	if (Button_GetIdealSize(hwnd, &size) == TRUE)
	{
		::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
	}
}
GuruLib::Button::Button(HWND _hwnd)
{
	hwnd = _hwnd;
}
