#pragma once
#include"WndTest.h"
#include"ClientDataType.h"

WndTest::WndTest(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	InitWindow(window_caption, parent_hwnd, height, width, type);
}

long WndTest::OnCreate(unsigned int wParam, long lParam)
{
	InitComponents();
	return 0;
}
long WndTest::OnClose(unsigned int wParam, long lParam)
{
	int ret(MessageBox(L"Are you sure to quit?", MB_YESNO));

	if (ret == IDYES)
	{
		::DestroyWindow(hwnd);
	}
	return 0;
}
long WndTest::OnNCDestroy(unsigned int wParam, long lParam)
{
	PostQuitMessage(0);
	return 0;
}
long WndTest::OnButtonTestClick(unsigned int wParam, long lParam)
{
	MessageBox(L"wow");
	return 0;
}

long WndTest::OnMenuChild1Click(unsigned int wParam, long lParam)
{
	MessageBox(L"child1");
	return 0;
}
long WndTest::OnMenuChild2Click(unsigned int wParam, long lParam)
{
	MessageBox(L"child2");
	return 0;
}
void WndTest::InitComponents()
{
	Wnd::CenterWindow(hwnd);
	TreeView tree_view = TreeView(7, 0, 61, 115, DIALOG_UNIT);
	tree_view.InsertItemA("parent1");
	tree_view.InsertItemA("parent2");
	tree_view.InsertItemA("child1", "parent1");
	tree_view.InsertItemA("child1", "parent2");
	tree_view.InsertItemA("child11", "child1");
	
	Menu menu;
	menu.InsertTtem(L"root");
	menu.InsertTtem(L"child1", L"root");
	AddHandler(MN_CLICKED, menu.GetResourceID(L"child1"), static_cast<MessageHandler>(&WndTest::OnMenuChild1Click));

	menu.InsertTtem(L"child2", L"root");
	AddHandler(MN_CLICKED, menu.GetResourceID(L"child2"), static_cast<MessageHandler>(&WndTest::OnMenuChild2Click));
}
void WndTest::InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	lpWindowName = window_caption;
	y = x = CW_USEDEFAULT;
	Height = height;
	Width = width;
	hwndParent = parent_hwnd;
	AddHandler(WM_CREATE, static_cast<MessageHandler>(&WndTest::OnCreate));
	AddHandler(WM_CLOSE, static_cast<MessageHandler>(&WndTest::OnClose));
	AddHandler(WM_NCDESTROY, static_cast<MessageHandler>(&WndTest::OnNCDestroy));
	_CreateWindow(type);

	
}

