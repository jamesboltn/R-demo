#pragma once
#include"TreeView.h"
//TODO:remove WS_BORDER style from some controls
//TreeView::TreeView(RECT* rect)
//{
//	lpClassName = WC_TREEVIEW;
//	dwStyle = WS_VISIBLE | WS_CHILD | TVS_HASLINES;
//	y = rect->top;
//	x = rect->left;
//	Height = rect->bottom - rect->top;
//	Width = rect->right - rect->left;
//	hwndParent = parent_hwnd;
//	hMenu = HMENU(resource_id);
//	// --- again those were supposed to be protected members of AbstractWindow --- 
//	_CreateWindow();
//}

TVINSERTSTRUCT tvins;
TVINSERTSTRUCTA tvins_a;

TreeView::TreeView(int x_pos, int y_pos, int width, int height, UnitType type)
{
	lpClassName = WC_TREEVIEW;
	dwStyle = WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_DISABLEDRAGDROP;
	y = y_pos;
	x = x_pos;
	Height = height;
	Width = width;
	hMenu = HMENU(resource_id);
	// --- again those were supposed to be protected members of AbstractWindow --- 
	_CreateWindow(type);

	//init members of TVINSERTSTRUCT and TVINSERTSTRUCTA
	tvins.item.mask = TVIF_TEXT;
	tvins_a.item.mask = TVIF_TEXT;
}
void TreeView::InsertItem(wchar_t* text, wchar_t* parent_item_text)
{
	tvins.item.pszText = text;
	tvins.item.cchTextMax = ARRAYCOUNT(text);
	
	// Set the parent item based on the specified level. 
	if (parent_item_text == NULL)
	{
		tvins.hInsertAfter = TVI_ROOT;
		tvins.hParent = NULL;
		// Add the item to the tree-view control. 
		item = (HTREEITEM)SendMessage(hwnd, TVM_INSERTITEM,
			0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		if (item != NULL)
		{
			text_item_map.insert(std::make_pair(text, item));
		}
	}
	else
	{
		
		auto it = text_item_map.find(parent_item_text);
		if (it != text_item_map.end())
		{
			tvins.hParent = it->second;
			tvins.hInsertAfter = (HTREEITEM)TVI_LAST;
			// Add the item to the tree-view control. 
			item = (HTREEITEM)SendMessage(hwnd, TVM_INSERTITEM,
				0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
			if (item != NULL)
			{
				text_item_map.insert(std::make_pair(text, item));
			}
		}	
	}
}
void TreeView::InsertItemA(char* text, char* parent_item_text)
{
	tvins_a.item.pszText = text;
	tvins_a.item.cchTextMax = ARRAYCOUNT(text);

	// Set the parent item based on the specified level. 
	if (parent_item_text == NULL)
	{
		tvins_a.hInsertAfter = TVI_ROOT;
		tvins_a.hParent = NULL;
		// Add the item to the tree-view control. 
		item = (HTREEITEM)SendMessageA(hwnd, TVM_INSERTITEMA,
			0, (LPARAM)(LPTVINSERTSTRUCTA)&tvins_a);
		if (item != NULL)
		{
			text_item_map_a.insert(std::make_pair(text, item));
		}
	}
	else
	{
		auto it = text_item_map_a.find(parent_item_text);
		if (it != text_item_map_a.end())
		{
			tvins_a.hParent = it->second;
			tvins_a.hInsertAfter = (HTREEITEM)TVI_LAST;
			// Add the item to the tree-view control. 
			item = (HTREEITEM)SendMessageA(hwnd, TVM_INSERTITEMA,
				0, (LPARAM)(LPTVINSERTSTRUCTA)&tvins_a);
			if (item != NULL)
			{
				text_item_map_a.insert(std::make_pair(text, item));
			}
		}
	}
}

