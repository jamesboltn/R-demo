#pragma once
#include"PreInclude.h"
#include"UIBase.h"
#include"WindowClass.h"

GuruLib::WindowClass* GuruLib::UIBase::window_class = new WindowClass();
HINSTANCE GuruLib::UIBase::hInstance = UIBase::window_class->hInstance;
HWND GuruLib::UIBase::last_created_window_hwnd = NULL;
HWND GuruLib::UIBase::GetLastCreatedWindow()
{
	return last_created_window_hwnd; 
}

GuruLib::UIBase::UIBase()
{
}
void GuruLib::UIBase::SetHWND(HWND _hwnd)
{
	hwnd = _hwnd;
}
int GuruLib::UIBase::GetClientAreaHeight()
{
	RECT client_rect;
	::GetClientRect(hwnd, &client_rect);
	return client_rect.bottom;
}
int GuruLib::UIBase::GetClientAreaWidth()
{
	RECT client_rect;
	::GetClientRect(hwnd, &client_rect);
	return client_rect.right;
}
HWND GuruLib::UIBase::GetHWND()
{
	return hwnd;
}
HWND GuruLib::UIBase::GetParentHWND()
{
	return parent_hwnd;
}
int GuruLib::UIBase::GetClientAreaXPos()
{
	POINT p = { 0 };
	::MapWindowPoints(hwnd, parent_hwnd, &p, 1);
	return p.x;
}
int GuruLib::UIBase::GetClientAreaYPos()
{
	POINT p = { 0 };
	::MapWindowPoints(hwnd, parent_hwnd, &p, 1);
	return p.y;
}
RECT GuruLib::UIBase::GetClientAreaRECT()
{
	RECT rect;
	::GetWindowRect(hwnd, &rect);
	::MapWindowPoints(HWND_DESKTOP, parent_hwnd, (LPPOINT)&rect, 2);
	return rect;
}
void GuruLib::UIBase::Show(HWND _hwnd)
{
	::ShowWindowAsync(_hwnd, SW_SHOW);
}
void GuruLib::UIBase::Hide(HWND _hwnd)
{
	::ShowWindowAsync(_hwnd, SW_HIDE);
}
void GuruLib::UIBase::Hide()
{
	::ShowWindowAsync(hwnd, SW_HIDE);
}
void GuruLib::UIBase::Show()
{
	::ShowWindowAsync(hwnd, SW_SHOW);
}
void GuruLib::UIBase::Enable(HWND _hwnd)
{
	::EnableWindow(_hwnd, TRUE);
}
void GuruLib::UIBase::Disable(HWND _hwnd)
{
	::EnableWindow(_hwnd, FALSE);
}
void GuruLib::UIBase::Disable()
{
	::EnableWindow(hwnd, FALSE);
}
void GuruLib::UIBase::Enable()
{
	::EnableWindow(hwnd, TRUE);
}
void GuruLib::UIBase::SetText(const wchar_t* text)
{
	::SetWindowTextW(hwnd, text);
}
void GuruLib::UIBase::SetTextA(const char* text)
{
	::SetWindowTextA(hwnd, text);
}
void GuruLib::UIBase::GetText(wchar_t* buffer, int length)
{
	::Button_GetText(hwnd, buffer, length);
}
void GuruLib::UIBase::GetTextA(char* buffer, int length)
{
	::GetWindowTextA(hwnd, buffer, length);
}
bool GuruLib::UIBase::_CreateWindow(UnitType type)
{
	//if the to be created window is a REAL WINDOW, should register a WM_CREATE handler before calling _CreateWindow
	if (type == DIALOG_UNIT)
	{
		height = DialogUnitToPixel(height, Y);
		width = DialogUnitToPixel(width, X);
		x = DialogUnitToPixel(x, X);
		y = DialogUnitToPixel(y, Y);
	}
	if (wcscmp(window_class_name, window_class->lpszClassName) != 0)
	{
		parent_hwnd = last_created_window_hwnd;
		
		hwnd = ::CreateWindowEx(
			extended_style_flag,
			window_class_name,
			window_name,
			style_flag,
			x,
			y,
			width,
			height,
			parent_hwnd,
			hMenu,
			hInstance,
			this                    // pointer to this class instance
		);

	}
	else
	{
		hwnd = ::CreateWindowEx(
			extended_style_flag,
			window_class_name,
			window_name,
			style_flag,
			x,
			y,
			width,
			height,
			parent_hwnd,
			hMenu,
			hInstance,
			this                    // pointer to this class instance
		);
	}

	if (!hwnd)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int GuruLib::UIBase::DialogUnitToPixel(int unit, Dimension d)
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
void GuruLib::UIBase::SetAsParent(HWND parent_hwnd)
{
	::SetParent(hwnd, parent_hwnd);
}
void GuruLib::UIBase::MoveWindowTo(int xPos, int yPos)
{
	::MoveWindow(hwnd, xPos, yPos, this->GetClientAreaWidth(), this->GetClientAreaHeight(), TRUE);
}
BOOL GuruLib::UIBase::IsEnabled()
{
	return ::IsWindowEnabled(hwnd);
}
void GuruLib::UIBase::AddToolTip(wchar_t* tip)
{
	if (hwnd_tooltip == NULL)
	{
		// Create a tooltip
		// A tooltip control should not have the WS_CHILD style, nor should it 
		// have an id, otherwise its behavior will be adversely affected.
		hwnd_tooltip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent_hwnd, 0, hInstance, 0);

		// Associate the tooltip with the button control
		tool_info = { 0 };
		tool_info.cbSize = sizeof(tool_info);
		tool_info.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		tool_info.uId = (UINT_PTR)hwnd;
		tool_info.hwnd = parent_hwnd;
	}

	// Associate the tooltip with the button control
	tool_info.lpszText = tip;
	SendMessage(hwnd_tooltip, TTM_ADDTOOL, 0, (LPARAM)&tool_info);
}
int GuruLib::UIBase::GetWidth()
{
	return width;
}
int GuruLib::UIBase::GetHeight()
{
	return height;
}