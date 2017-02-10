#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"WindowClass.h"

WindowClass* UIBase::window_class = new WindowClass();

HINSTANCE UIBase::hInstance = UIBase::window_class->hInstance;

HWND UIBase::last_created_window_hwnd = NULL;

UIBase::UIBase()
{
	lpWindowName = NULL;
	dwExStyle = 0;
}

void UIBase::SetHWND(HWND _hwnd)
{
	hwnd = _hwnd;
}

int UIBase::GetClientAreaHeight()
{
	RECT client_rect;
	::GetClientRect(hwnd, &client_rect);
	return client_rect.bottom;
}

int UIBase::GetClientAreaWidth()
{
	RECT client_rect;
	::GetClientRect(hwnd, &client_rect);
	return client_rect.right;
}

HWND UIBase::GetHWND()
{
	return hwnd;
}

HWND UIBase::GetParentHWND(HWND child_hwnd)
{
	return hwndParent;
}

int UIBase::GetClientAreaXPos()
{
	POINT p = { 0 };
	::MapWindowPoints(hwnd, hwndParent, &p, 1);
	return p.x;
}

int UIBase::GetClientAreaYPos()
{
	POINT p = { 0 };
	::MapWindowPoints(hwnd, hwndParent, &p, 1);
	return p.y;
}

RECT UIBase::GetClientAreaRECT()
{
	RECT rect;
	::GetWindowRect(hwnd, &rect);
	::MapWindowPoints(HWND_DESKTOP, hwndParent, (LPPOINT)&rect, 2);
	return rect;
}

void UIBase::Show(HWND _hwnd)
{
	::ShowWindowAsync(_hwnd, SW_SHOW);
}

void UIBase::Hide(HWND _hwnd)
{
	::ShowWindowAsync(_hwnd, SW_HIDE);
}

void UIBase::Hide()
{
	::ShowWindowAsync(hwnd, SW_HIDE);
}

void UIBase::Show()
{
	::ShowWindowAsync(hwnd, SW_SHOW);
}

void UIBase::Enable(HWND _hwnd)
{
	::EnableWindow(_hwnd, TRUE);
}

void UIBase::Disable(HWND _hwnd)
{
	::EnableWindow(_hwnd, FALSE);
}

void UIBase::SetText(const wchar_t* text)
{
	::SetWindowTextW(hwnd, text);
}

void UIBase::SetTextA(const char* text)
{
	::SetWindowTextA(hwnd, text);
}

void UIBase::GetText(wchar_t* buffer, int length)
{
	::Button_GetText(hwnd, buffer, length);
}

void UIBase::GetTextA(char* buffer, int length)
{
	::GetWindowTextA(hwnd, buffer, length);
}

bool UIBase::_CreateWindow(UnitType type)
{
	//if the to be created window is a REAL WINDOW, should register a WM_CREATE handler before calling _CreateWindow
	if (type == DIALOG_UNIT)
	{
		Height = DialogUnitToPixel(Height, Y);
		Width = DialogUnitToPixel(Width, X);
		x = DialogUnitToPixel(x, X);
		y = DialogUnitToPixel(y, Y);
	}
	if (wcscmp(lpClassName, window_class->lpszClassName) != 0)
	{
		hwndParent = last_created_window_hwnd;
		hwnd = ::CreateWindowEx(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			Width,
			Height,
			hwndParent,
			hMenu,
			hInstance,
			this                    // pointer to this class instance
		);
	}
	else
	{
		hwnd = ::CreateWindowEx(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			Width,
			Height,
			hwndParent,
			hMenu,
			hInstance,
			this                    // pointer to this class instance
		);
	}

	if (!hwnd)
		return false;
	else
	{
		return true;
	}
}

int UIBase::DialogUnitToPixel(int unit, Dimension d)
{
	if (d == HORIZONTAL)
	{
		return MulDiv(unit, LOWORD(GetDialogBaseUnits()), 4);
	}
	else
	{
		//VERTICAL
		return MulDiv(unit, HIWORD(GetDialogBaseUnits()), 8);
	}
}

void UIBase::SetAsParent(HWND parent_hwnd)
{
	::SetParent(hwnd, parent_hwnd);
}

void UIBase::MoveWindowTo(int xPos, int yPos)
{
	::MoveWindow(hwnd, xPos, yPos, this->GetClientAreaWidth(), this->GetClientAreaHeight(), TRUE);
}
