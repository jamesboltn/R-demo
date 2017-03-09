#include"preinclude.h"
#include"ControlBase.h"
#include"Window.h"
#include"WindowClass.h"
#include"Utilities.h"

#define MN_CLICKED 0
std::unordered_set<unsigned int> Window::command_messages
({
	// menu
	MN_CLICKED,

	//button
	BN_CLICKED,

	//edit
	EN_KILLFOCUS,

	//combobox
	CBN_DROPDOWN,
	CBN_CLOSEUP,
	CBN_SELCHANGE,
	CBN_SELENDOK,
	CBN_SELENDCANCEL,

	//listbox
	LBN_SELCHANGE
});

std::unordered_set<unsigned int> Window::notify_messages
({
	//tab
	TCN_SELCHANGE,

	//datagrid(list view)
	LVN_GETDISPINFO,
	NM_CLICK,//parent window notified when an row(item) of datagrid(treeview) is single clicked
	NM_DBLCLK,//parent window notified when an row(item) is double clicked 
	NM_KILLFOCUS,
	NM_RCLICK,
	NM_RDBLCLK,
	NM_RETURN,
	NM_SETFOCUS,
	LVN_ENDLABELEDIT,
	LVN_BEGINLABELEDIT,
	LVN_GETINFOTIP,//parent window notified when an row(item) is hovered on
	LVN_ITEMCHANGING,//parent window notified when an row(item) is changing(being clicked)
	HDN_ITEMSTATEICONCLICK,//parent window notified when datagrid header checkbox is clicked 
});

//local function for Window::app_init_flag
bool Window::UseCommonControls()
{
#pragma region use common controls
	//ensure this app can use common controls
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_ANIMATE_CLASS |
		ICC_BAR_CLASSES |
		ICC_COOL_CLASSES |
		ICC_DATE_CLASSES |
		ICC_HOTKEY_CLASS |
		ICC_INTERNET_CLASSES |
		ICC_LINK_CLASS |
		ICC_LISTVIEW_CLASSES |
		ICC_NATIVEFNTCTL_CLASS |
		ICC_PAGESCROLLER_CLASS |
		ICC_PROGRESS_CLASS |
		ICC_STANDARD_CLASSES |
		ICC_TAB_CLASSES |
		ICC_TREEVIEW_CLASSES |
		ICC_UPDOWN_CLASS |
		ICC_USEREX_CLASSES |
		ICC_WIN95_CLASSES;
	::InitCommonControlsEx(&iccx);
#pragma endregion
	return true;
}

bool Window::app_init_flag(UseCommonControls());

Window::Window(void)
{
	lpClassName = window_class->lpszClassName;
	dwStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX | WS_CLIPSIBLINGS | WS_VISIBLE;
	hMenu = NULL;
	AddHandler(WM_CLOSE, &Window::OnClose);
	AddHandler(TCN_SELCHANGE, &Window::OnTabSelectionChanging);
	AddHandler(LVN_BEGINLABELEDIT, &Window::OnDataGridCellBeginEdit);
	AddHandler(LVN_ENDLABELEDIT, &Window::OnDataGridCellEndEdit);
	AddHandler(TVN_SELCHANGING, &Window::OnTreeViewSelectionChanging);
}

Window::~Window(void)
{
}

typedef WindowMessageMap::iterator WindowMessageMapIterator;
/*********************type definition for WM_COMMAND messages*****************************/
typedef CommandMessageMap::iterator CommandMessageMapIterator;
/*********************type definition for WM_NOTIFY messages***************************/
typedef NotifyMessageMap::iterator NotifyMessageMapIterator;
unsigned int notify_message_combination = TCN_SELCHANGING | LVN_BEGINLABELEDIT | LVN_ENDLABELEDIT | LVN_GETINFOTIP | LVN_ITEMCHANGING | TVN_SELCHANGING | TVN_SELCHANGED;
LRESULT CALLBACK Window::MessageRouter(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	Window* wnd = NULL;

	if (message == WM_NCCREATE)
	{
		// retrieve Window instance from window creation data and associate
		wnd = reinterpret_cast<Window*>((LPCREATESTRUCT(lParam))->lpCreateParams);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<long>(wnd));

		// save window handle
		wnd->SetHWND(hwnd);
		UIBase::last_created_window_hwnd = hwnd;
	}
	else
		// retrieve associated Window instance
		wnd = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (wnd)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			CommandMessageMapIterator it((wnd->command_message_map).find(std::make_pair(HIWORD(wParam), LOWORD(wParam))));
			if (it != (wnd->command_message_map).end())
			{
				return (wnd->*(it->second))(wParam, lParam);
			}
		}
		break;
		case WM_NOTIFY:
		{
			LPNMHDR lpnmhdr((LPNMHDR)lParam);

			if (lpnmhdr->code & notify_message_combination)
			{
				WindowMessageMapIterator it = (wnd->window_message_map).find(lpnmhdr->code);
				if (it != (wnd->window_message_map).end())
				{
					return (wnd->*(it->second))(wParam, lParam);
				}
			}
			else
			{
				NotifyMessageMapIterator it((wnd->notify_message_map).find(std::make_pair(lpnmhdr->code, lpnmhdr->idFrom)));
				if (it != (wnd->notify_message_map).end())
				{
					return (wnd->*(it->second))(wParam, lParam);
				}
			}
		}
		break;
		default:
		{
			WindowMessageMapIterator it = (wnd->window_message_map).find(message);
			if (it != (wnd->window_message_map).end())
			{
				return (wnd->*(it->second))(wParam, lParam);
			}
		}
		}

	}
	return DefWindowProcW(hwnd, message, wParam, lParam);
}

int Window::PumpMessages()
{
	MSG  msg;
	while (::GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

void Window::MakeModal()
{
	HWND parent_hwnd(GetParentHWND(hwnd));
	if (!::IsWindow(parent_hwnd))
	{
		return;
	}
	Disable(parent_hwnd);

}

// Window::OnClose is a method called in response to WM_CLOSE
long Window::OnClose(unsigned int wParam, long lParam)
{
	//enable parent window if current window is a modal child one
	HWND parent_hwnd(GetParentHWND(hwnd));
	if (::IsWindow(parent_hwnd))
	{
		if (!::IsWindowEnabled(parent_hwnd))
		{
			Enable(parent_hwnd);
		}
	}
	DestroyWindow(hwnd);
	return 0;
}

void Window::AddHandler(unsigned int message, MessageHandler handler)
{
	if (window_message_map.find(message) == window_message_map.end())
		window_message_map.insert(std::pair<MessageID, MessageHandler>(message, handler));
	else
		window_message_map[message] = handler;
}

void Window::AddHandler(unsigned int message, ControlBase* control, MessageHandler handler)
{
	if (command_messages.find(message) != command_messages.end())
	{
		CommandMessageID command_message_id(std::make_pair(message, control->resource_id));
		if (command_message_map.find(command_message_id) == command_message_map.end())
			command_message_map.insert(std::pair<CommandMessageID, MessageHandler>(command_message_id, handler));
	}
	else if (notify_messages.find(message) != notify_messages.end())
	{
		NotifyMessageID notify_message_id(std::make_pair(message, control->resource_id));
		if (notify_message_map.find(notify_message_id) == notify_message_map.end())
			notify_message_map.insert(std::pair<NotifyMessageID, MessageHandler>(notify_message_id, handler));
	}
}

void Window::AddHandler(unsigned int message, int resource_id, MessageHandler handler)
{
	if (command_messages.find(message) != command_messages.end())
	{
		CommandMessageID command_message_id(std::make_pair(message, resource_id));
		if (command_message_map.find(command_message_id) == command_message_map.end())
			command_message_map.insert(std::pair<CommandMessageID, MessageHandler>(command_message_id, handler));
	}
	else if (notify_messages.find(message) != notify_messages.end())
	{
		NotifyMessageID notify_message_id(std::make_pair(message, resource_id));
		if (notify_message_map.find(notify_message_id) == notify_message_map.end())
			notify_message_map.insert(std::pair<NotifyMessageID, MessageHandler>(notify_message_id, handler));
	}
}

wchar_t* Wnd::GetOpenFilePath(wchar_t* filter_pattern, HWND hwnd, bool can_multi_select, wchar_t* dialog_title, wchar_t* initial_dir)
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

wchar_t* Wnd::GetSaveFolderPath(wchar_t* filter_pattern, wchar_t* file_name, HWND hwnd, wchar_t* dialog_title, wchar_t* initial_dir)
{
	wchar_t file_path[MAX_PATH];
	wcscpy_s(file_path, ARRAYCOUNT(file_path), file_name);
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

void Wnd::CenterWindow(HWND hwnd)
{
	RECT window, desktop;
	::GetWindowRect(hwnd, &window);
	::GetWindowRect(GetDesktopWindow(), &desktop);
	::MoveWindow(hwnd, ((desktop.right - desktop.left) - (window.right - window.left)) / 2,
		((desktop.bottom - desktop.top) - (window.bottom - window.top)) / 2,
		(window.right - window.left), (window.bottom - window.top), 0);
}

void Wnd::SetWindowTopmost(HWND hwnd)
{
	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Wnd::SetWindowNoTopmost(HWND hwnd)
{
	::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool Wnd::IsWindowTopmost(HWND hwnd)
{
	if (::GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
		return true;
	else
		return false;
}

void Wnd::MinimizeWindow(HWND hwnd)
{
	::ShowWindow(hwnd, SW_MINIMIZE);
}

void Wnd::MaximizeWindow(HWND hwnd)
{
	::ShowWindow(hwnd, SW_MAXIMIZE);
}

bool Wnd::IsWindowMinimized(HWND hwnd)
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

bool Wnd::IsWindowMaximized(HWND hwnd)
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

void Wnd::SetTranslucentLevel(HWND hwnd, BYTE translucent_level)
{
	::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ::GetWindowLongPtr(hwnd, GWL_STYLE) | WS_EX_LAYERED);

	::SetLayeredWindowAttributes(hwnd, 255, translucent_level, LWA_ALPHA);
}

void Wnd::FlashTitleBar(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_CAPTION | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}

void Wnd::FlashTaskBarButton(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}

void Wnd::Flash(HWND hwnd)
{
	FLASHWINFO fwi;
	fwi.cbSize = sizeof(fwi);
	fwi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fwi.dwTimeout = 0;
	fwi.hwnd = hwnd;
	fwi.uCount = INFINITE;

	FlashWindowEx(&fwi);
}

int Window::MessageBox(wchar_t* text, wchar_t* caption, unsigned type)
{
	if (caption == NULL)
	{
		return ::MessageBox(hwnd, text, UIBase::window_class->messagebox_title, type);
	}
	else
	{
		return ::MessageBox(hwnd, text, caption, type);
	}
}

int Window::MessageBox(wchar_t* text, unsigned type)
{
	return ::MessageBox(hwnd, text, UIBase::window_class->messagebox_title, type);
}

int Window::MessageBox(wchar_t* text)
{
	return ::MessageBox(hwnd, text, UIBase::window_class->messagebox_title, 0);
}

int Window::MessageBoxA(char* text)
{
	std::string title_a;
	TypeConverter::WCharStrToString(UIBase::window_class->messagebox_title, &title_a);
	return ::MessageBoxA(hwnd, text, title_a.c_str(), 0);
}

void Window::SetAsMainWindow()
{
	UIBase::window_class->messagebox_title = lpWindowName;
}