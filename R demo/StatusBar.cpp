#pragma once
#include"preinclude.h"
#include"StatusBar.h"

StatusBar::StatusBar(std::vector<WidthPercent>* width_percents)
{
	lpClassName = STATUSCLASSNAME;
	dwStyle = WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP;
	y = 0;
	x = 0;
	Height = 0;
	Width = 0;
	hMenu = HMENU(resource_id);
	//again those were supposed to be protected members of AbstractWindow --- 
	_CreateWindow();

	RECT rcClient;
	HLOCAL hloc;
	PINT paParts;
	// Get the coordinates of the parent window's client area.
	::GetClientRect(hwndParent, &rcClient);

	// Allocate an array for holding the right edge coordinates.
	hloc = ::LocalAlloc(LHND, sizeof(int) * width_percents->size());
	paParts = (PINT)::LocalLock(hloc);

	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	int count(0);
	for (auto it = width_percents->begin(); it != width_percents->end(); ++it)
	{
		if ((count - 1) >= 0)
		{
			paParts[count] = paParts[count - 1];
			paParts[count] += static_cast<int>((double)*it / 100.0 * (double)rcClient.right);
		}
		else
		{
			paParts[count] = static_cast<int>((double)*it / 100.0 * (double)rcClient.right);
		}
		++count;
	}

	// Tell the status bar to create the window parts.
	::SendMessage(hwnd, SB_SETPARTS, (WPARAM)width_percents->size(), (LPARAM)
		paParts);

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);
}

void StatusBar::SetBlockText(unsigned int block_index, wchar_t* text)
{
	::SendMessage(hwnd, SB_SETTEXT, (WPARAM)block_index, (LPARAM)text);
}

void StatusBar::SetBlockTextA(unsigned int block_index, char* text)
{
	::SendMessageA(hwnd, SB_SETTEXTA, (WPARAM)block_index, (LPARAM)text);
}

void StatusBar::AutoSize()
{
	::SendMessage(hwnd, WM_SIZE, 0, 0);
}