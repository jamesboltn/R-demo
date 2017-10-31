#pragma once
#include"ListBox.h"

GuruLib::ListBox::ListBox(int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	window_class_name = L"LISTBOX";
	style_flag = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY;
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
GuruLib::ListBox::ListBox(HWND _hwnd)
{
	hwnd = _hwnd;
}
void GuruLib::ListBox::AddString(const std::vector<wchar_t*> &items)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		ListBox_AddString(hwnd, *it);
	}
}
void GuruLib::ListBox::AddString(const wchar_t* str)
{
	ListBox_AddString(hwnd, str);
}
void GuruLib::ListBox::AddStringA(const char* str)
{
	::SendMessageA(hwnd, LB_ADDSTRING, 0L, (LPARAM)(const char*)(str));
}
int GuruLib::ListBox::GetSelectIndex()
{
	return ::SendMessage(hwnd, LB_GETCURSEL, 0, 0);
}
