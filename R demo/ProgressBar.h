#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

typedef int LOW, HIGH;
class ProgressBar :
	public UIBase,
	public ControlBase
{
public:
	ProgressBar(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	ProgressBar(HWND _hwnd);//accepts HWND of created ProgressBar
	ProgressBar() {}
	virtual ~ProgressBar() {}
	void SetRange(int min, int max);
	std::pair<LOW, HIGH> GetRange();
};