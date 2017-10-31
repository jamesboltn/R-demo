#pragma once
#include"CheckBox.h"

GuruLib::CheckBox::CheckBox(const wchar_t* caption, int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	//we're going to assume the following members are declared in Window as protected
	window_name = caption;
	window_class_name = L"BUTTON";
	style_flag = WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX;
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
GuruLib::CheckBox::CheckBox(HWND _hwnd)
{
	hwnd = _hwnd;
}
bool GuruLib::CheckBox::Checked()
{
	unsigned  short status = ::SendMessage(hwnd, BM_GETCHECK, NULL, NULL);
	if (status == BST_CHECKED)
	{
		return true;
	}
	else if (status == BST_INDETERMINATE || status == BST_UNCHECKED)
	{
		return false;
	}
}
void GuruLib::CheckBox::Check()
{
	Button_SetCheck(hwnd, BST_CHECKED);
}
void GuruLib::CheckBox::UnCheck()
{
	Button_SetCheck(hwnd, BST_UNCHECKED);
}