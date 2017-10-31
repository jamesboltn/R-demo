#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlMessageHandler.h"

namespace GuruLib
{
	/************************type definition for other WM_XXX messages***********************/
	typedef std::unordered_map<MessageID, MessageHandler> WindowMessagePrePostExecuteMap;
	typedef std::unordered_map<MessageID, std::function<long(WPARAM wParam, LPARAM lParam)> > WindowMessageMap;

#define AddHandler(MsgID, Handler) _AddHandler(this, MsgID, &##Handler)

	class Window :
		public UIBase, public ControlMessageHandler
	{
	private:
		WindowMessageMap window_message_map;
		WindowMessagePrePostExecuteMap window_message_pre_execute_map;
		WindowMessagePrePostExecuteMap window_message_post_execute_map;
		void AddPreExecuteHandler(unsigned int message, MessageHandler handler);
		void AddPostExecuteHandler(unsigned int message, MessageHandler handler);

		static bool UseCommonControls();

#pragma region default message handlers
		//default handlers for window
		long OnClose(WPARAM wParam, LPARAM lParam);
		long OnCreate(WPARAM wParam, LPARAM lParam);//pre_execute this func to center the window relative to its parent window or to the desktop
		long OnResize(WPARAM wParam, LPARAM lParam);//auto resize and reposition the child controls on the window when it is resized
		long OnNCDestroy(WPARAM wParam, LPARAM lParam);//post_execute this func to finally destroy the window
													   //default handlers for tab
		long OnTabSelectionChanging(WPARAM wParam, LPARAM lParam);
		//default handlers for datagrid
		long OnDataGridCellBeginEdit(WPARAM wParam, LPARAM lParam);
		long OnDataGridCellEndEdit(WPARAM wParam, LPARAM lParam);
		//default handlers for treeview
		long OnTreeViewSelectionChanging(WPARAM wParam, LPARAM lParam);
#pragma endregion
	protected:
		static bool app_init_flag;

		LIB int MessageBox(wchar_t* text, wchar_t* caption, unsigned type);
		LIB int MessageBox(wchar_t* text, unsigned type);
		LIB int MessageBox(wchar_t* text);
		LIB int MessageBoxFormat(wchar_t* caption, unsigned type, wchar_t* format, ...);
		LIB int MessageBoxFormat(unsigned type, wchar_t* format, ...);
		LIB int MessageBoxFormat(wchar_t* format, ...);
		LIB int MessageBoxA(char* text);

		virtual void InitComponents() = 0;
		virtual void InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type) = 0;
	public:
		LIB Window();
		LIB virtual ~Window();

		template<typename DerivedWindow>
		void _AddHandler(DerivedWindow* d_wnd, unsigned int message, long (DerivedWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			using namespace std::placeholders; //for _1, _2, _3...
			window_message_map[message] = std::bind(handler, d_wnd, _1, _2);
		}

		void MakeModal();
		LIB void SetAsMainWindow();
		static LRESULT CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);
		LIB static int MessageLoop();
	};
}
