#pragma once
#include"preinclude.h"
#include"ComboBox.h"

ComboBox::ComboBox(int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpClassName = L"COMBOBOX";
	dwStyle = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL | CBS_DROPDOWNLIST;
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

void ComboBox::AddString(wchar_t* item)
{
	::SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));
}

void ComboBox::AddStringA(char* item)
{
	::SendMessageA(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));
}

void ComboBox::Remove(unsigned int index)
{
	::SendMessage(hwnd, CB_DELETESTRING, index, 0);
}

void ComboBox::Clear()
{
	::SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

void ComboBox::ShowDropDown()
{
	::SendMessage(hwnd, CB_SHOWDROPDOWN, TRUE, 0);
}

void ComboBox::SetSelection(unsigned int index)
{
	::SendMessage(hwnd, CB_SETCURSEL, index, 0);
}

int ComboBox::GetDropDownWidth()
{
	return ::SendMessage(hwnd, CB_GETDROPPEDWIDTH, 0, 0);
}

void ComboBox::SetDropDownWidth(unsigned int width)
{
	::SendMessage(hwnd, CB_SETDROPPEDWIDTH, width, 0);
}

int ComboBox::GetSelectIndex()
{
	return ::SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void ComboBox::GetSelectString(wchar_t* buffer, unsigned int length)
{
	::SendMessage(hwnd, WM_GETTEXT, length, (LPARAM)buffer);
}

void ComboBox::GetSelectStringA(char* buffer, unsigned int length)
{
	::SendMessageA(hwnd, WM_GETTEXT, length, (LPARAM)buffer);
}

bool ComboBox::IsListDroppedDown()
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

ComboBox::ComboBox(HWND _hwnd)
{
	hwnd = _hwnd;
}