#pragma once
#include"Button.h"

Button::Button(wchar_t* button_text, int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	// --- we're going to assume the following members are declared in Window as protected
	lpWindowName = button_text;
	lpClassName = L"BUTTON";
	dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
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
Button::Button(wchar_t* caption, RECT* rect, char* _unique_name)
{
	// --- we're going to assume the following members are declared in Window as protected
	lpWindowName = caption;
	lpClassName = L"BUTTON";
	dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	y = rect->top;
	x = rect->left;
	Height = rect->bottom - rect->top;
	Width = rect->right - rect->left;
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
void Button::SetText (const wchar_t* button_text) 
{
	::Button_SetText(hwnd, button_text);

	//repaint button to fit its content
	SIZE size;
	if (Button_GetIdealSize(hwnd, &size) == TRUE)
	{
		::MoveWindow(hwnd, x, y, (int)size.cx, (int)size.cy, TRUE);
	}
}

Button::Button(HWND _hwnd)
{
	hwnd = _hwnd;
}