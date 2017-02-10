#pragma once
#pragma once
#include"Window.h"
#include"TreeView.h"
#include"Button.h"
#include"Menu.h"

class WndTest :
	public Window
{
private:
	long OnCreate(unsigned int wParam, long lParam);
	long OnClose(unsigned int wParam, long lParam);
	long OnNCDestroy(unsigned int wParam, long lParam);
	long OnButtonTestClick(unsigned int wParam, long lParam);

	long OnMenuChild1Click(unsigned int wParam, long lParam);
	long OnMenuChild2Click(unsigned int wParam, long lParam);
protected:
	void InitComponents();
	void InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type);
public:
	WndTest(wchar_t* window_caption, HWND parent_hwnd = NULL, int height = CW_USEDEFAULT, int width = CW_USEDEFAULT, UnitType type = PIXEL);
};


