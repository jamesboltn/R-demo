#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class Static :
	public UIBase,
	public ControlBase
{
private:
public:
	Static(wchar_t* text, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	Static(HWND _hwnd);//accepts HWND of created Static
	Static() {}
	virtual ~Static() {}
	virtual void SetText(const wchar_t* text);
};

