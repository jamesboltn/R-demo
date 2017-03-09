#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class Button :
	public UIBase,
	public ControlBase
{
private:
public:
	Button(wchar_t* caption, int x_pos = 0, int y_pos = 0, int width = 50, int height = 50, UnitType type = PIXEL, char* _unique_name = NULL);
	Button(wchar_t* caption, RECT* rect, char* _unique_name = NULL);
	Button(HWND _hwnd);//accepts HWND of created Button
	virtual void SetText (const wchar_t* button_text);
	Button() {}
};
