#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"DefaultHandler.h"

class Window;
typedef long(Window::*MessageHandler)(unsigned int wParam, long lParam);
typedef unsigned int MessageID;
typedef std::pair<MessageID, ResourceID> CommandMessageID, NotifyMessageID;
typedef std::map<std::pair<MessageID, ResourceID>, MessageHandler> CommandMessageMap, NotifyMessageMap;
/************************type definition for other WM_XXX messages***********************/
typedef std::unordered_map<MessageID, MessageHandler> WindowMessageMap;

class ControlBase;
class Window :
	private DefaultHandler, public UIBase
{
private:
	WindowMessageMap window_message_map;
	CommandMessageMap command_message_map;
	NotifyMessageMap notify_message_map;
	long OnClose(unsigned int wParam, long lParam);
	static std::unordered_set<unsigned int> command_messages;
	static std::unordered_set<unsigned int> notify_messages;
	static bool UseCommonControls();
protected:
	static bool app_init_flag;
	void AddHandler(unsigned int message, MessageHandler handler);
	void AddHandler(unsigned int message, ControlBase* control, MessageHandler handler);
	void AddHandler(unsigned int message, int resource_id, MessageHandler handler);
	virtual void InitComponents() = 0;
	virtual void InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type) = 0;
public:
	Window();
	virtual ~Window();
	int MessageBox(wchar_t* text, wchar_t* caption, unsigned type);
	int MessageBox(wchar_t* text, unsigned type);
	int MessageBox(wchar_t* text);
	int MessageBoxA(char* text);
	void MakeModal();
	void SetAsMainWindow();
	static LRESULT CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);
	static int PumpMessages();
};

namespace Wnd
{
	//L"All Files(*.*)\0*.log\0"
	//"TestFiles(*.1,*.2;*.3;*.4...;*.200)\0*.1;*.2;*.3;*.4;*.5;*.6...;*.200\0All Files(*.*)\0*.*\0"
	//L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	wchar_t* GetOpenFilePath(wchar_t* filter_pattern, HWND hwnd, bool can_multi_select = false, wchar_t* dialog_title = NULL, wchar_t* initial_dir = NULL);
	wchar_t* GetSaveFolderPath(wchar_t* filter_pattern, wchar_t* file_name, HWND hwnd, wchar_t* dialog_title = NULL, wchar_t* initial_dir = NULL);
	void CenterWindow(HWND hwnd);
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
