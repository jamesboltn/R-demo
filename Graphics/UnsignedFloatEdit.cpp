#pragma once
#include"UnsignedFloatEdit.h"
#include"StringAssist.h"

static WNDPROC unsigned_float_proc = NULL;

static LRESULT CALLBACK UnsignedFloatProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (wParam == '.')
		{
			wchar_t buffer[MAX_PATH];
			Edit_GetText(hwnd, buffer, MAX_PATH);
			if (GuruLib::StdWString::Contains(&std::wstring(buffer), &std::wstring(L".")))
			{
				return 0;
			}
		}
		else if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_RETURN
			|| wParam == VK_DELETE
			|| wParam == VK_BACK))
		{
			return 0;
		}
	}

	return ::CallWindowProc(unsigned_float_proc, hwnd, uMsg, wParam, lParam);
}

GuruLib::UnsignedFloatEdit::UnsignedFloatEdit(int x_pos, int y_pos, int _width, int _height, UnitType type)
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
		WNDPROC edit_proc = reinterpret_cast<WNDPROC>(SubclassWindow(hwnd, reinterpret_cast<LONG_PTR>(UnsignedFloatProc)));
		if (unsigned_float_proc == NULL)
		{
			unsigned_float_proc = edit_proc;
		}
	}
}
GuruLib::UnsignedFloatEdit::UnsignedFloatEdit(HWND _hwnd)
{
	hwnd = _hwnd;
}