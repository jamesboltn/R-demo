#pragma once
#include"preinclude.h"
#include"DataTypeForUI.h"

#define MN_CLICKED 0
#define WM_DESTROYING WM_DESTROY

typedef long ResourceID;

class WindowClass;
class UIBase
{
private:
	static int DialogUnitToPixel(int unit, Dimension d);
protected:
	/***::CreateWindowEx() parameters***/
	DWORD dwExStyle = 0;
	LPCWSTR lpClassName;
	DWORD dwStyle;
	HWND hwndParent;
	HMENU hMenu; 
	HWND hwnd;
	int x;
	int y;
	int Width;
	int Height;
	LPCWSTR lpWindowName = NULL;
	/******/
	static HINSTANCE hInstance;
	static WindowClass* window_class;
	static HWND last_created_window_hwnd;
	virtual bool _CreateWindow(UnitType type = PIXEL);
	void SetHWND(HWND _hwnd);
public:
	UIBase();
	int GetClientAreaHeight();
	int GetClientAreaWidth();
	int GetClientAreaXPos();
	int GetClientAreaYPos();
	RECT GetClientAreaRECT();
	HWND GetHWND();
	HWND GetParentHWND(HWND child_hwnd);
	void SetAsParent(HWND parent_hwnd);
	void Show();//show itself
	void Show(HWND _hwnd);//show other windows or itself
	void Hide(HWND _hwnd);//hide other windows or itself
	void Hide();//hide itsell
	void Enable(HWND _hwnd);
	void Disable(HWND _hwnd);
	void MoveWindowTo(int xPos, int yPos);
	virtual void SetText(const wchar_t* text);
	virtual void SetTextA(const char* text);
	void GetText(wchar_t* buffer, int length);
	void GetTextA(char* buffer, int length);
};