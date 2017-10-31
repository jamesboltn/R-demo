#pragma once
#include"ComboBox.h"

GuruLib::ComboBox::ComboBox(int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	window_class_name = L"COMBOBOX";
	style_flag = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL | CBS_DROPDOWNLIST;
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
GuruLib::ComboBox::ComboBox(HWND _hwnd)
{
	hwnd = _hwnd;
}
void GuruLib::ComboBox::AddString(const wchar_t* item)
{
	::SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));
}
void GuruLib::ComboBox::AddStringA(const char* item)
{
	::SendMessageA(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));
}
void GuruLib::ComboBox::Remove(unsigned int index)
{
	::SendMessage(hwnd, CB_DELETESTRING, index, 0);
}
void GuruLib::ComboBox::Clear()
{
	::SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}
void GuruLib::ComboBox::ShowDropDown()
{
	::SendMessage(hwnd, CB_SHOWDROPDOWN, TRUE, 0);
}
void GuruLib::ComboBox::SetSelection(unsigned int index)
{
	::SendMessage(hwnd, CB_SETCURSEL, index, 0);
}
int GuruLib::ComboBox::GetDropDownWidth()
{
	return ::SendMessage(hwnd, CB_GETDROPPEDWIDTH, 0, 0);
}
void GuruLib::ComboBox::SetDropDownWidth(unsigned int width)
{
	::SendMessage(hwnd, CB_SETDROPPEDWIDTH, width, 0);
}
int GuruLib::ComboBox::GetSelectIndex()
{
	return ::SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}
void GuruLib::ComboBox::GetSelectString(wchar_t* buffer, unsigned int length)
{
	::SendMessage(hwnd, WM_GETTEXT, length, (LPARAM)buffer);
}
void GuruLib::ComboBox::GetSelectStringA(char* buffer, unsigned int length)
{
	::SendMessageA(hwnd, WM_GETTEXT, length, (LPARAM)buffer);
}
bool GuruLib::ComboBox::IsListDroppedDown()
{
	if (::SendMessage(hwnd, CB_GETDROPPEDSTATE, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}
