#pragma once
#include"preinclude.h"
#include"ListBox.h"

ListBox::ListBox(int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpClassName = L"LISTBOX";
	dwStyle = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY;
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

}

void ListBox::AddString(std::vector<wchar_t*>* items)
{
	for (auto it = items->begin(); it != items->end(); ++it)
	{
		ListBox_AddString(hwnd, *it);
	}
}

void ListBox::AddString(wchar_t* str)
{
	ListBox_AddString(hwnd, str);
}

void ListBox::AddStringA(char* str)
{
	::SendMessageA(hwnd, LB_ADDSTRING, 0L, (LPARAM)(const char*)(str));
}

int ListBox::GetSelectIndex()
{
	return ::SendMessage(hwnd, LB_GETCURSEL, 0, 0);
}

ListBox::ListBox(HWND _hwnd)
{
	hwnd = _hwnd;
}