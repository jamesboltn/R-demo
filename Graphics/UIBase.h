#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"GraphicsDef.h"

namespace GuruLib
{
	class WindowClass;
	class UIBase
	{
	private:
		static int DialogUnitToPixel(int unit, Dimension d);
		HWND hwnd_tooltip = NULL;
		TOOLINFO tool_info;
	protected:
		/***::CreateWindowEx() parameters***/
		DWORD extended_style_flag = 0;
		LPCWSTR window_class_name;
		DWORD style_flag;
		HWND parent_hwnd;
		HMENU hMenu;
		HWND hwnd;
		int x;
		int y;
		int width;
		int height;
		LPCWSTR window_name = NULL;
		/******/
		static HINSTANCE hInstance;
		static WindowClass* window_class;
		static HWND last_created_window_hwnd;
#ifdef DLL_EXPORT
		LIB
#endif
			virtual bool _CreateWindow(UnitType type = PIXEL);
		void SetHWND(HWND _hwnd);
	public:
		UIBase();

#ifdef DLL_EXPORT
		LIB
#endif
			static HWND GetLastCreatedWindow();
#ifdef DLL_EXPORT
		LIB
#endif
			int GetClientAreaHeight();
		int GetClientAreaWidth();
		int GetClientAreaXPos();
		int GetClientAreaYPos();
		RECT GetClientAreaRECT();
#ifdef DLL_EXPORT
		LIB
#endif
			HWND GetHWND();
		HWND GetParentHWND();
		void SetAsParent(HWND parent_hwnd);
#ifdef DLL_EXPORT
		LIB
#endif
			void Show();//show itself
#ifdef DLL_EXPORT
		LIB
#endif
			void Show(HWND _hwnd);//show other windows or itself
		void Hide(HWND _hwnd);//hide other windows or itself
#ifdef DLL_EXPORT
		LIB
#endif
			void Hide();//hide itsell
		void Enable(HWND _hwnd);
		void Disable(HWND _hwnd);
#ifdef DLL_EXPORT
		LIB
#endif 
			void Disable();
#ifdef DLL_EXPORT
		LIB
#endif 
			void Enable();
		void MoveWindowTo(int xPos, int yPos);
#ifdef DLL_EXPORT
		LIB
#endif
			virtual void SetText(const wchar_t* text);
#ifdef DLL_EXPORT
		LIB
#endif 
			virtual void SetTextA(const char* text);
#ifdef DLL_EXPORT
		LIB
#endif 
			void GetText(wchar_t* buffer, int length);
		void GetTextA(char* buffer, int length);
#ifdef DLL_EXPORT
		LIB
#endif 
			BOOL IsEnabled();
#ifdef DLL_EXPORT
		LIB
#endif
			void AddToolTip(wchar_t* tip);
#ifdef DLL_EXPORT
		LIB
#endif
			int GetWidth();
#ifdef DLL_EXPORT
		LIB
#endif
			int GetHeight();
	};
}
