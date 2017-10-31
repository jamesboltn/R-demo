#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class Button :
		public UIBase,
		public ControlBase
	{
	private:
	public:
		LIB Button() {}
		LIB Button(const wchar_t* caption, int x_pos = 0, int y_pos = 0, int width = 50, int height = 50, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB Button(const wchar_t* caption, const RECT &rect, const char* _unique_name = NULL);
		LIB explicit Button(HWND _hwnd);//accepts HWND of created Button

		LIB virtual void SetText(const wchar_t* button_text);

		template<typename HostWindow>
		LIB inline void OnClick(long (HostWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			AddCtrlWindowHandler(BN_CLICKED, this, static_cast<MessageHandler>(handler));
		}

		template<typename HostWindow>
		LIB inline void OnClick(HostWindow* host_wnd, long (HostWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			AddCtrlPageHandler(BN_CLICKED, this, handler, host_wnd);
		}
	};
}
