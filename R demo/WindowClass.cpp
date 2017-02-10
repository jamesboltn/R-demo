#pragma once
#include"preinclude.h"
#include"WindowClass.h"
#include"Window.h"

WindowClass::WindowClass()
{
	hInstance = GetModuleHandle(NULL);
	// all messages for windows belonging to this Window Class will get sent to msgRouter
	lpfnWndProc = Window::MessageRouter;
	lpszClassName = L"window_class";

	// --- set default values for the rest of the WNDCLASSEX structure ---
	// --- later you can derive your own class and modify this behavior ---
	lpszMenuName = NULL;
	cbSize = sizeof(WNDCLASSEX);
	cbClsExtra = 0;
	cbWndExtra = 0;
	style = 0;
	hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	hCursor = ::LoadCursor(NULL, IDC_ARROW);
	hbrBackground = CreateSolidBrush(RGB(240, 240, 240));/*(HBRUSH)(COLOR_WINDOW + 1)*/;

	::RegisterClassEx(this);
}