#pragma once
#include"Edit.h"
#include"Utilities.h"
#include"FileSystem.h"
#include"DateTime.h"
#include"StringAssist.h"

GuruLib::Edit::Edit()
{
}
GuruLib::Edit::Edit(int x_pos, int y_pos, int _width, int _height, UnitType type, EditScrollStyle scroll_style, EditLineStyle line_style, const char* _unique_name)
{
	extended_style_flag |= WS_EX_CLIENTEDGE;
	window_class_name = L"EDIT";
	style_flag = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	if (line_style == SINGLE_LINE)
	{
		style_flag &= ~ES_MULTILINE;
	}
	if (scroll_style == HAS_SCROLLBAR)
	{
		style_flag |= WS_VSCROLL | WS_HSCROLL;
	}
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
}
GuruLib::Edit::Edit(const RECT &rect, UnitType type, EditScrollStyle scroll_style, EditLineStyle line_style, const char* _unique_name)
{
	extended_style_flag |= WS_EX_CLIENTEDGE;
	window_class_name = L"EDIT";
	style_flag = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	if (line_style == SINGLE_LINE)
	{
		style_flag &= ~ES_MULTILINE;
	}
	if (scroll_style == HAS_SCROLLBAR)
	{
		style_flag |= WS_VSCROLL | WS_HSCROLL;
	}
	y = rect.top;
	x = rect.left;
	height = rect.bottom - rect.top;
	width = rect.right - rect.left;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}
void GuruLib::Edit::SetReadOnly(bool is_read_only)
{
	if (is_read_only)
	{
		Edit_SetReadOnly(hwnd, TRUE);
	}
	else
	{
		Edit_SetReadOnly(hwnd, FALSE);
	}
}
void GuruLib::Edit::SetMask(const wchar_t* mask)
{
	Edit_SetCueBannerText(hwnd, mask);
}
void GuruLib::Edit::GetMask(wchar_t* mask, int length)
{
	Edit_GetCueBannerText(hwnd, mask, length);
}
void GuruLib::Edit::AppendLine(const wchar_t* text)
{
	wchar_t line[512];
	wcscpy_s(line, ArraySize(line), text);
	wcscat_s(line, ArraySize(line), L"\r\n");
	int length(::GetWindowTextLength(hwnd));
	::SendMessage(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)line);
}
void GuruLib::Edit::AppendLineA(const char* text, bool include_timestamp)
{
	char line[512];
	if (include_timestamp)
	{
		DateTime dt;
		strcpy_s(line, ArraySize(line), dt.Current().ToTimeString().c_str());
		strcat_s(line, ArraySize(line), " ");
		strcat_s(line, ArraySize(line), text);
	}
	else
	{
		strcpy_s(line, ArraySize(line), text);
	}
	strcat_s(line, ArraySize(line), "\r\n");
	int length(::GetWindowTextLengthA(hwnd));
	::SendMessageA(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)(const char*)(line));
}
void GuruLib::Edit::AppendText(const wchar_t* text)
{
	int length(::GetWindowTextLength(hwnd));
	::SendMessage(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)text);
}
GuruLib::Edit::Edit(HWND _hwnd)
{
	hwnd = _hwnd;
}
