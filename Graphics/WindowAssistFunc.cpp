#pragma once
#include"WindowAssistFunc.h"

wchar_t* GuruLib::WindowAssistFunc::GetOpenFilePath(wchar_t* filter_pattern, HWND hwnd, bool can_multi_select, wchar_t* dialog_title, wchar_t* initial_dir)
{
	wchar_t file_path[MAX_PATH];
	OPENFILENAMEW ofn;
	::ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;

	ofn.lpstrFile = file_path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file_path);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON;

	if (can_multi_select == true)
		ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (dialog_title != NULL)
		ofn.lpstrTitle = dialog_title;
	if (initial_dir != NULL)
		ofn.lpstrInitialDir = initial_dir;

	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	ofn.lpstrFilter = filter_pattern;

	if (::GetOpenFileNameW(&ofn))
	{
		return ofn.lpstrFile;
	}
	else
	{
		return NULL;
	}
}
wchar_t* GuruLib::WindowAssistFunc::GetSaveFolderPath(wchar_t* filter_pattern, wchar_t* file_name, HWND hwnd, wchar_t* dialog_title, wchar_t* initial_dir)
{
	wchar_t file_path[MAX_PATH];
	wcscpy_s(file_path, ArraySize(file_path), file_name);
	OPENFILENAMEW ofn;
	::ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;

	ofn.lpstrFile = file_path;
	ofn.nMaxFile = sizeof(file_path);
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON;

	if (dialog_title != NULL)
		ofn.lpstrTitle = dialog_title;
	if (initial_dir != NULL)
		ofn.lpstrInitialDir = initial_dir;

	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	ofn.lpstrFilter = filter_pattern;

	if (::GetSaveFileNameW(&ofn))
	{
		return ofn.lpstrFile;
	}
	else
	{
		return NULL;
	}

}
void GuruLib::WindowAssistFunc::CenterWindow(HWND hwnd, HWND parent_hwnd)
{
	RECT rc1, rc2;
	RECT FAR *lprc;
	int nWidth, nHeight, cxCenter, cyCenter;
	if (!::IsWindow(parent_hwnd))
	{
		parent_hwnd = ::GetDesktopWindow();
	}

	::GetWindowRect(parent_hwnd, &rc2);
	lprc = (RECT FAR *)&rc2;

	cxCenter = lprc->left + ((lprc->right - lprc->left) >> 1);
	cyCenter = lprc->top + ((lprc->bottom - lprc->top) >> 1);

	::GetWindowRect(hwnd, &rc1);
	nWidth = rc1.right - rc1.left;
	nHeight = rc1.bottom - rc1.top;

	::MoveWindow(hwnd, 
		cxCenter-(nWidth >> 1),
		cyCenter-(nHeight >> 1),
		nWidth,
		nHeight,
		FALSE);
}
void GuruLib::WindowAssistFunc::SetWindowTopmost(HWND hwnd)
{
	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
void GuruLib::WindowAssistFunc::SetWindowNoTopmost(HWND hwnd)
{
	::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
bool GuruLib::WindowAssistFunc::IsWindowTopmost(HWND hwnd)
{
	if (::GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
		return true;
	else
		return false;
}
void GuruLib::WindowAssistFunc::MinimizeWindow(HWND hwnd)
{
	::ShowWindow(hwnd, SW_MINIMIZE);
}
void GuruLib::WindowAssistFunc::MaximizeWindow(HWND hwnd)
{
	::ShowWindow(hwnd, SW_MAXIMIZE);
}
bool GuruLib::WindowAssistFunc::IsWindowMinimized(HWND hwnd)
{
	if (::IsIconic(hwnd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GuruLib::WindowAssistFunc::IsWindowMaximized(HWND hwnd)
{
	if (::IsZoomed(hwnd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void GuruLib::WindowAssistFunc::SetTranslucentLevel(HWND hwnd, BYTE translucent_level)
{
	::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ::GetWindowLongPtr(hwnd, GWL_STYLE) | WS_EX_LAYERED);

	::SetLayeredWindowAttributes(hwnd, 255, translucent_level, LWA_ALPHA);
}
void GuruLib::WindowAssistFunc::FlashTitleBar(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_CAPTION | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}
void GuruLib::WindowAssistFunc::FlashTaskBarButton(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}
void GuruLib::WindowAssistFunc::Flash(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}