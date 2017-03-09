#pragma once
#include"preinclude.h"
#include"ProgressBar.h"

ProgressBar::ProgressBar(int x_pos, int y_pos, int width, int height, UnitType type, char* _unique_name)
{
	lpClassName = PROGRESS_CLASS;
	dwStyle = WS_VISIBLE | WS_CHILD | PBS_SMOOTH;
	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	dwExStyle = 0;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}

void ProgressBar::SetRange(int min, int max)
{
	::SendMessage(GetHWND(), PBM_SETRANGE, 0, MAKELPARAM(min, max));
}

std::pair<LOW, HIGH> ProgressBar::GetRange()
{
	int low = ::SendMessage(GetHWND(), PBM_GETRANGE, TRUE, NULL);
	int high = ::SendMessage(GetHWND(), PBM_GETRANGE, FALSE, NULL);
	return std::make_pair(low, high);
}