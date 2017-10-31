#include"Utilities.h"
#include"ControlBase.h"
#include"Window.h"
#include"WindowClass.h"
#include"WindowAssistFunc.h"

//local function for Window::app_init_flag
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' ""version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
bool GuruLib::Window::UseCommonControls()
{
	//use common controls
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = 
		ICC_ANIMATE_CLASS      |
		ICC_BAR_CLASSES        |
		ICC_COOL_CLASSES       |
		ICC_DATE_CLASSES       |
		ICC_HOTKEY_CLASS       |
		ICC_INTERNET_CLASSES   |
		ICC_LINK_CLASS         |
		ICC_LISTVIEW_CLASSES   |
		ICC_NATIVEFNTCTL_CLASS |
		ICC_PAGESCROLLER_CLASS |
		ICC_PROGRESS_CLASS     |
		ICC_STANDARD_CLASSES   |
		ICC_TAB_CLASSES        |
		ICC_TREEVIEW_CLASSES   |
		ICC_UPDOWN_CLASS       |
		ICC_USEREX_CLASSES     |
		ICC_WIN95_CLASSES;
	auto ret = ::InitCommonControlsEx(&iccx);
	return ret;
}

bool GuruLib::Window::app_init_flag = UseCommonControls();

GuruLib::Window::Window(void)
{
	window_class_name = window_class->lpszClassName;
	style_flag = WS_OVERLAPPEDWINDOW /*^ WS_THICKFRAME ^ WS_MAXIMIZEBOX*/ | WS_CLIPSIBLINGS | WS_VISIBLE;
	hMenu = NULL;
	AddHandler(WM_CLOSE, Window::OnClose);
	AddPreExecuteHandler(WM_CREATE, &Window::OnCreate);
	AddPreExecuteHandler(WM_SIZE, &Window::OnCreate);
	AddPostExecuteHandler(WM_NCDESTROY, &Window::OnNCDestroy);
	AddHandler(TCN_SELCHANGE, Window::OnTabSelectionChanging);
	AddHandler(LVN_BEGINLABELEDIT, Window::OnDataGridCellBeginEdit);
	AddHandler(LVN_ENDLABELEDIT, Window::OnDataGridCellEndEdit);
	AddHandler(TVN_SELCHANGING, Window::OnTreeViewSelectionChanging);
}

GuruLib::Window::~Window(void)
{
}

static unsigned int notify_message_combination = TCN_SELCHANGING | LVN_BEGINLABELEDIT | LVN_ENDLABELEDIT | LVN_GETINFOTIP | LVN_ITEMCHANGING | TVN_SELCHANGING | TVN_SELCHANGED;
LRESULT CALLBACK GuruLib::Window::MessageRouter(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	Window* wnd = NULL;

	if (message == WM_NCCREATE)
	{
		// retrieve Window instance from window creation data and associate
		wnd = reinterpret_cast<Window*>((LPCREATESTRUCT(lParam))->lpCreateParams);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
		
		// save window handle
		wnd->SetHWND(hwnd);
		UIBase::last_created_window_hwnd = hwnd;
	}
	else
	{
		// retrieve associated Window instance
		wnd = reinterpret_cast<Window*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (wnd)
		{
			switch (message)
			{
			case WM_COMMAND:
			{
				auto it = (wnd->command_message_map).find({ HIWORD(wParam), LOWORD(wParam) });
				if (it != (wnd->command_message_map).end())
				{
					return it->second(wParam, lParam);
				}
			}
			break;
			case WM_NOTIFY:
			{
				LPNMHDR lpnmhdr((LPNMHDR)lParam);
				if (lpnmhdr->code & notify_message_combination)
				{
					auto it = (wnd->window_message_map).find(lpnmhdr->code);
					if (it != (wnd->window_message_map).end())
					{
						return it->second(wParam, lParam);
					}
				}
				else
				{
					auto it = (wnd->notify_message_map).find({ lpnmhdr->code, lpnmhdr->idFrom });
					if (it != (wnd->notify_message_map).end())
					{
						return it->second(wParam, lParam);
					}
				}
			}
			break;
			default:
			{
				auto it = (wnd->window_message_pre_execute_map).find(message);
				if (it != (wnd->window_message_pre_execute_map).end())
				{
					auto ret = (wnd->*(it->second))(wParam, lParam);

					auto it2 = (wnd->window_message_map).find(message);
					if (it2 != (wnd->window_message_map).end())
					{
						auto ret1 = it2->second(wParam, lParam);

						it = (wnd->window_message_post_execute_map).find(message);
						if (it != (wnd->window_message_post_execute_map).end())
						{
							return (wnd->*(it->second))(wParam, lParam);
						}
						else
						{
							return ret1;
						}
					}
					else
					{
						it = (wnd->window_message_post_execute_map).find(message);

						if (it != (wnd->window_message_post_execute_map).end())
						{
							return (wnd->*(it->second))(wParam, lParam);
						}
						else
						{
							return ret;
						}
					}
				}
				else
				{
					auto it2 = (wnd->window_message_map).find(message);
					if (it2 != (wnd->window_message_map).end())
					{
						auto ret = it2->second(wParam, lParam);

						it = (wnd->window_message_post_execute_map).find(message);
						if (it != (wnd->window_message_post_execute_map).end())
						{
							return (wnd->*(it->second))(wParam, lParam);
						}
						else
						{
							return ret;
						}
					}
					else
					{
						it = (wnd->window_message_post_execute_map).find(message);

						if (it != (wnd->window_message_post_execute_map).end())
						{
							return (wnd->*(it->second))(wParam, lParam);
						}
					}
				}
			}
			break;
			}
		}
	}
	return DefWindowProcW(hwnd, message, wParam, lParam);
}

int GuruLib::Window::MessageLoop()
{
	MSG  msg;
	while (::GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

void GuruLib::Window::MakeModal()
{
	HWND parent_hwnd = GetParentHWND();
	if (!::IsWindow(parent_hwnd))
	{
		return;
	}
	Disable(parent_hwnd);

}

long GuruLib::Window::OnClose(WPARAM wParam, LPARAM lParam)
{
	//enable parent window if current window is a modal child one
	HWND parent_hwnd = GetParentHWND();
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

long GuruLib::Window::OnCreate(WPARAM wParam, LPARAM lParam)
{
	WindowAssistFunc::CenterWindow(hwnd, parent_hwnd);	
	return 0;
}

long GuruLib::Window::OnResize(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

long GuruLib::Window::OnNCDestroy(WPARAM wParam, LPARAM lParam)
{
	if (!::IsWindow(parent_hwnd))
	{
		::PostQuitMessage(0);
	}
	else
	{
		::PostMessage(parent_hwnd, MSG_CHILD_WINDOW_DESTROY, NULL, (LPARAM)hwnd);
	}
	return 0;
}

void GuruLib::Window::AddPreExecuteHandler(unsigned int message, MessageHandler handler)
{
	if (window_message_pre_execute_map.find(message) == window_message_pre_execute_map.end())
	{
		window_message_pre_execute_map.insert(std::pair<MessageID, MessageHandler>(message, handler));
	}
	else
	{
		window_message_pre_execute_map[message] = handler;
	}
}

void GuruLib::Window::AddPostExecuteHandler(unsigned int message, MessageHandler handler)
{
	if (window_message_post_execute_map.find(message) == window_message_post_execute_map.end())
	{
		window_message_post_execute_map.insert(std::pair<MessageID, MessageHandler>(message, handler));
	}
	else
	{
		window_message_post_execute_map[message] = handler;
	}
}

void GuruLib::Window::SetAsMainWindow()
{
	UIBase::window_class->messagebox_title = window_name;
}

long GuruLib::Window::OnDataGridCellBeginEdit(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

long GuruLib::Window::OnDataGridCellEndEdit(WPARAM wParam, LPARAM lParam)
{
	LPNMLVDISPINFOW pdi((LPNMLVDISPINFOW)lParam);

	if (pdi->item.pszText)
	{
		ListView_SetItemText(pdi->hdr.hwndFrom, pdi->item.iItem, 0, pdi->item.pszText);// put new text
		return TRUE;
	}
	return FALSE;
}

long GuruLib::Window::OnTabSelectionChanging(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

long GuruLib::Window::OnTreeViewSelectionChanging(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

int GuruLib::Window::MessageBox(wchar_t* text, wchar_t* caption, unsigned type)
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

int GuruLib::Window::MessageBox(wchar_t* text, unsigned type)
{
	return ::MessageBox(hwnd, text, UIBase::window_class->messagebox_title, type);
}

int GuruLib::Window::MessageBox(wchar_t* text)
{
	return ::MessageBox(hwnd, text, UIBase::window_class->messagebox_title, 0);
}

int GuruLib::Window::MessageBoxFormat(wchar_t* caption, unsigned type, wchar_t* format, ...)
{
	wchar_t buffer[1024];
	va_list arg_list;
	va_start(arg_list, format);
	_vsnwprintf(buffer, ArraySize(buffer), format, arg_list);
	va_end(arg_list);

	if (caption == NULL)
	{
		return ::MessageBox(hwnd, buffer, UIBase::window_class->messagebox_title, type);
	}
	else
	{
		return ::MessageBox(hwnd, buffer, caption, type);
	}
}

int GuruLib::Window::MessageBoxFormat(unsigned type, wchar_t* format, ...)
{
	wchar_t buffer[1024];
	va_list arg_list;
	va_start(arg_list, format);
	_vsnwprintf(buffer, ArraySize(buffer), format, arg_list);
	va_end(arg_list);

	return ::MessageBox(hwnd, buffer, UIBase::window_class->messagebox_title, type);
}

int GuruLib::Window::MessageBoxFormat(wchar_t* format, ...)
{
	wchar_t buffer[1024];
	va_list arg_list;
	va_start(arg_list, format);
	_vsnwprintf(buffer, ArraySize(buffer), format, arg_list);
	va_end(arg_list);

	return ::MessageBox(hwnd, buffer, UIBase::window_class->messagebox_title, 0);
}

int GuruLib::Window::MessageBoxA(char* text)
{
	std::string title_a;
	TypeConverter::WCharStrToString(UIBase::window_class->messagebox_title, &title_a);
	return ::MessageBoxA(hwnd, text, title_a.c_str(), 0);
}