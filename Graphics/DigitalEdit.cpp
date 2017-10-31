#pragma once
#include"DigitalEdit.h"

static WNDPROC digital_proc = NULL;

static LRESULT CALLBACK DigitalProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_RETURN
			|| wParam == VK_DELETE
			|| wParam == VK_BACK))
		{
			return 0;
		}
	}
	return ::CallWindowProc(digital_proc, hwnd, uMsg, wParam, lParam);
}

GuruLib::DigitalEdit::DigitalEdit(int x_pos, int y_pos, int _width, int _height, UnitType type)
{
	extended_style_flag |= WS_EX_CLIENTEDGE;
	window_class_name = L"EDIT";
	style_flag = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	y = y_pos;
	x = x_pos;
	height = _height;
	width = _width;
	hMenu = HMENU(resource_id);

	_CreateWindow(type);

	if (hwnd != NULL)
	{
		// subclass the window so we can filter keystrokes
		WNDPROC edit_proc = reinterpret_cast<WNDPROC>(SubclassWindow(hwnd, reinterpret_cast<LONG_PTR>(DigitalProc)));
		if (digital_proc == NULL)
		{
			digital_proc = edit_proc;
		}
	}
}
GuruLib::DigitalEdit::DigitalEdit(HWND _hwnd)
{
	hwnd = _hwnd;
}