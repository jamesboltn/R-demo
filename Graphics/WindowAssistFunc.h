#pragma once
#include"PreInclude.h"

namespace GuruLib
{
	namespace WindowAssistFunc
	{
		//L"All Files(*.*)\0*.log\0"
		//"TestFiles(*.1,*.2;*.3;*.4...;*.200)\0*.1;*.2;*.3;*.4;*.5;*.6...;*.200\0All Files(*.*)\0*.*\0"
		//L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		wchar_t* GetOpenFilePath(wchar_t* filter_pattern, HWND hwnd, bool can_multi_select = false, wchar_t* dialog_title = NULL, wchar_t* initial_dir = NULL);
		wchar_t* GetSaveFolderPath(wchar_t* filter_pattern, wchar_t* file_name, HWND hwnd, wchar_t* dialog_title = NULL, wchar_t* initial_dir = NULL);
		LIB void CenterWindow(HWND hwnd, HWND parent_hwnd = NULL);
		void SetWindowTopmost(HWND hwnd);
		void SetWindowNoTopmost(HWND hwnd);
		bool IsWindowTopmost(HWND hwnd);
		void MinimizeWindow(HWND hwnd);
		void MaximizeWindow(HWND hwnd);
		bool IsWindowMinimized(HWND hwnd);
		bool IsWindowMaximized(HWND hwnd);
		void FlashTitleBar(HWND hwnd);
		void FlashTaskBarButton(HWND hwnd);
		//flash both title bar and task bar button
		void Flash(HWND hwnd);
		//translucent range 0~255
		void SetTranslucentLevel(HWND hwnd, BYTE translucent_level);
	}
}

