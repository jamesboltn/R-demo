#pragma once
#include"preinclude.h"
#include"Edit.h"
#include"Utilities.h"

Edit::Edit()
{
}
Edit::Edit(int x_pos, int y_pos, int width, int height, EditLineStyle line_style, EditScrollStyle scroll_style, UnitType type, char* _unique_name)
{
	dwExStyle |= WS_EX_CLIENTEDGE;
	lpClassName = L"EDIT";
	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	if (line_style == SINGLE_LINE)
	{
		dwStyle &= ~ES_MULTILINE;
	}
	if (scroll_style == HAS_SCROLLBAR)
	{
		dwStyle |= WS_VSCROLL | WS_HSCROLL;
	}
	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id); 
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}
}
void Edit::SetReadOnly(bool is_read_only)
{
	if (is_read_only)
	{
		Edit_SetReadOnly(hwnd, TRUE);
	}
	else
	{
		Edit_SetReadOnly(hwnd, FALSE);
	}
}
void Edit::SetMask(wchar_t* mask)
{
	Edit_SetCueBannerText(hwnd, mask);
}
void Edit::GetMask(wchar_t* mask, int length)
{
	Edit_GetCueBannerText(hwnd, mask, length);
}
void Edit::AppendLine(wchar_t* text)
{
	wchar_t line[512];
	wcscpy_s(line, ARRAYCOUNT(line), text);
	wcscat_s(line, ARRAYCOUNT(line), L"\r\n");
	int length(::GetWindowTextLength(hwnd));
	::SendMessage(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)line);
}
void Edit::AppendLineA(const char* text, bool include_timestamp)
{
	char line[512];
	if (include_timestamp)
	{
		strcpy_s(line, ARRAYCOUNT(line), DateTime::GetCurrentTimeString().c_str());
		strcat_s(line, ARRAYCOUNT(line), " ");
		strcat_s(line, ARRAYCOUNT(line), text);
	}
	else
	{
		strcpy_s(line, ARRAYCOUNT(line), text);
	}
	strcat_s(line, ARRAYCOUNT(line), "\r\n");
	int length(::GetWindowTextLengthA(hwnd));
	::SendMessageA(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)(const char*)(line));
}
void Edit::AppendText(wchar_t* text)
{
	int length(::GetWindowTextLength(hwnd));
	::SendMessage(hwnd, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	::SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)text);
}
Edit::Edit(HWND _hwnd)
{
	hwnd = _hwnd;
}

static WNDPROC local_edit_proc1(NULL);

LRESULT CALLBACK UnsignedFloatProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (wParam == '.')
		{
			wchar_t buffer[MAX_PATH];
			Edit_GetText(hwnd, buffer, MAX_PATH);
			if (WString::Contains(&std::wstring(buffer), &std::wstring(L".")))
			{
				return 0;
			}
		}
		else if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_RETURN
			|| wParam == VK_DELETE
			|| wParam == VK_BACK))
		{
			return 0;
		}
	}
	return ::CallWindowProc(local_edit_proc1, hwnd, uMsg, wParam, lParam);
}

UnsignedFloatEdit::UnsignedFloatEdit(int x_pos, int y_pos, int width, int height, UnitType type)
{
	dwExStyle |= WS_EX_CLIENTEDGE;
	lpClassName = L"EDIT";
	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);

	_CreateWindow(type);

	if (hwnd != NULL)
	{
		// subclass the window so we can filter keystrokes
		WNDPROC edit_proc(reinterpret_cast<WNDPROC>(SetWindowLongPtr(
			hwnd,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(UnsignedFloatProc))));
		if (local_edit_proc1 == NULL)
		{
			local_edit_proc1 = edit_proc;
		}
	}
}
UnsignedFloatEdit::UnsignedFloatEdit(HWND _hwnd)
{
	hwnd = _hwnd;
}

static WNDPROC local_edit_proc2(NULL);

LRESULT CALLBACK DigitalProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_RETURN
			|| wParam == VK_DELETE
			|| wParam == VK_BACK))
		{
			return 0;
		}
	}
	return ::CallWindowProc(local_edit_proc2, hwnd, uMsg, wParam, lParam);
}

DigitalEdit::DigitalEdit(int x_pos, int y_pos, int width, int height, UnitType type)
{
	dwExStyle |= WS_EX_CLIENTEDGE;
	lpClassName = L"EDIT";
	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);

	_CreateWindow(type);

	if (hwnd != NULL)
	{
		// subclass the window so we can filter keystrokes
		WNDPROC edit_proc(reinterpret_cast<WNDPROC>(SetWindowLongPtr(
			hwnd,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(DigitalProc))));
		if (local_edit_proc2 == NULL)
		{
			local_edit_proc2 = edit_proc;
		}
	}
}

DigitalEdit::DigitalEdit(HWND _hwnd)
{
	hwnd = _hwnd;
}

EditWithLog::EditWithLog(int x_pos, int y_pos, int width, int height, UnitType type)
{
	dwExStyle |= WS_EX_CLIENTEDGE;
	lpClassName = L"EDIT";
	dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL;

	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);

	_CreateWindow(type);

	if (hwnd != NULL)
	{
		Edit_SetReadOnly(hwnd, TRUE);
	}
}

void EditWithLog::WriteLog(const char* msg, const char* folder_path)
{
	if (fp == NULL)
	{
		if (folder_path == NULL)
		{
			if (log_path == "")
			{
				log_path = "./Log/" + DateTime::GetCurrentDateString() + ".log";
			}
			else
			{
				log_path += DateTime::GetCurrentDateString() + ".log";
			}
		}
		else
		{
			log_path = folder_path + DateTime::GetCurrentDateString() + ".log";
		}
		fopen_s(&fp, log_path.c_str(), "a");
	}

	std::string time_stamp(DateTime::GetCurrentTimeString());
	std::string record(time_stamp + " " + msg);
	AppendLineA(record.c_str(), false);
	FileIO::WriteLog(msg, fp, &time_stamp);
}

void EditWithLog::WriteLogLocal(const char* msg, const char* folder_path)
{
	if (fp == NULL)
	{
		if (folder_path == NULL)
		{
			if (log_path == "")
			{
				log_path = "./Log/" + DateTime::GetCurrentDateString() + ".log";
			}
			else
			{
				log_path += DateTime::GetCurrentDateString() + ".log";
			}
		}
		else
		{
			log_path = folder_path + DateTime::GetCurrentDateString() + ".log";
		}
		fopen_s(&fp, log_path.c_str(), "a");
	}
	std::string time_stamp = DateTime::GetCurrentTimeString();
	FileIO::WriteLog(msg, fp, &time_stamp);
}

EditWithLog::EditWithLog(HWND _hwnd)
{
	hwnd = _hwnd;
}