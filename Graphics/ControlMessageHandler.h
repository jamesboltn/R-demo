#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"GraphicsDef.h"

namespace GuruLib
{
	static std::unordered_set<MessageID> command_messages
	({
		// menu
		MN_CLICKED,

		//button
		BN_CLICKED,

		//edit
		EN_KILLFOCUS,
		EN_CHANGE,

		//combobox
		CBN_DROPDOWN,
		CBN_CLOSEUP,
		CBN_SELCHANGE,
		CBN_SELENDOK,
		CBN_SELENDCANCEL,

		//listbox
		LBN_SELCHANGE
	});

	static std::unordered_set<MessageID> notify_messages
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

	class ControlBase;
	class ControlMessageHandler
	{
	private:
	protected:
		CommandMessageMap command_message_map;
		NotifyMessageMap notify_message_map;
	public:
		template<typename DerivedWindow>
		LIB inline void _AddControlHandler(DerivedWindow* d_wnd, unsigned int message, ControlBase* control, long (DerivedWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			using namespace std::placeholders; //for _1, _2, _3...
			if (command_messages.find(message) != command_messages.end())
			{
				CommandMessageID command_message_id({ message, control->resource_id });
				if (command_message_map.find(command_message_id) == command_message_map.end())
				{
					command_message_map[command_message_id] = std::bind(handler, d_wnd, _1, _2);
				}
			}
			else if (notify_messages.find(message) != notify_messages.end())
			{
				NotifyMessageID notify_message_id({ message, control->resource_id });
				if (notify_message_map.find(notify_message_id) == notify_message_map.end())
				{
					notify_message_map[notify_message_id] = std::bind(handler, d_wnd, _1, _2);
				}
			}
		}

		template<typename DerivedWindow>
		LIB inline void _AddControlHandler(DerivedWindow* d_wnd, unsigned int message, int resource_id, long (DerivedWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			using namespace std::placeholders; //for _1, _2, _3...
			if (command_messages.find(message) != command_messages.end())
			{
				CommandMessageID command_message_id({ message, resource_id });
				if (command_message_map.find(command_message_id) == command_message_map.end())
				{
					command_message_map[command_message_id] = std::bind(handler, d_wnd, _1, _2);
				}
			}
			else if (notify_messages.find(message) != notify_messages.end())
			{
				NotifyMessageID notify_message_id({ message, resource_id });
				if (notify_message_map.find(notify_message_id) == notify_message_map.end())
				{
					notify_message_map[notify_message_id] = std::bind(handler, d_wnd, _1, _2);
				}
			}
		}
	};

}
