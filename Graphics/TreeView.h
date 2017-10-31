#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	class TreeView :
		public UIBase,
		public ControlBase
	{
	private:
		HTREEITEM item;
		std::unordered_map<std::string, HTREEITEM> text_item_map_a;
		std::unordered_map<std::wstring, HTREEITEM> text_item_map;

		void GetItemText(HTREEITEM item, char* buffer, unsigned int length);
	public:
		LIB TreeView(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
		TreeView() {}

		LIB void InsertItem(wchar_t* text, const wchar_t* parent_item_text = NULL);//level is zero-based. if parent_item_text is NULL, then root item is inserted
		LIB void InsertItemA(char* text, const char* parent_item_text = NULL);//level is zero-based. if parent_item_text is NULL, then root item is inserted
		LIB void GetSelectedItemText(char* buffer, unsigned int length);
		LIB void GetParentOfSelectedItemText(char* buffer, unsigned int length);
		LIB void GetChainedTextOfSelectedItem(char* buffer, unsigned int length);

		template<typename HostWindow>
		LIB void OnSelectionChanged(long (HostWindow::*handler)(WPARAM wParam, LPARAM lParam))
		{
			AddWindowHandler(TVN_SELCHANGED, static_cast<MessageHandler>(handler));
		}

		template<typename HostWindow>
		LIB void OnSelectionChanged(long (HostWindow::*handler)(WPARAM wParam, LPARAM lParam), HostWindow* host_wnd)
		{
			AddPageHandler(TVN_SELCHANGED, handler, host_wnd);
		}
	};

}
