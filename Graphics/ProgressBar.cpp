#pragma once
#include"PreInclude.h"
#include"ProgressBar.h"

GuruLib::ProgressBar::ProgressBar(int x_pos, int y_pos, int _width, int _height, UnitType type, const char* _unique_name)
{
	window_class_name = PROGRESS_CLASS;
	style_flag = WS_VISIBLE | WS_CHILD | PBS_SMOOTH;
	y = y_pos;
	x = x_pos;
	height = height;
	width = width;
	extended_style_flag = 0;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}
GuruLib::ProgressBar::ProgressBar(HWND _hwnd)
{
	hwnd = _hwnd;
}
void GuruLib::ProgressBar::SetRange(int min, int max)
{
	::SendMessage(GetHWND(), PBM_SETRANGE, 0, MAKELPARAM(min, max));
}
std::pair<GuruLib::LOW, GuruLib::HIGH> GuruLib::ProgressBar::GetRange()
{
	int low = ::SendMessage(GetHWND(), PBM_GETRANGE, TRUE, NULL);
	int high = ::SendMessage(GetHWND(), PBM_GETRANGE, FALSE, NULL);
	return std::make_pair(low, high);
}