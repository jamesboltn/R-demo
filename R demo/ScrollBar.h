#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class ScrollBar :
	public UIBase,
	public ControlBase
{
public:
	ScrollBar(ScrollBarStyle style, int x_pos, int y_pos, int width, int height, char* _unique_name = NULL);
	ScrollBar(HWND _hwnd);//accepts HWND of created ScrollBar
	ScrollBar() {}
	virtual ~ScrollBar() {}
};