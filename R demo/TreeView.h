#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

class TreeView :
	public UIBase,
	public ControlBase
{
private:
	HTREEITEM item;
	std::unordered_map<std::string, HTREEITEM> text_item_map_a;
	std::unordered_map<std::wstring, HTREEITEM> text_item_map;
public:
	//TreeView(RECT* rect);
	TreeView(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
	TreeView() {}
	void InsertItem(wchar_t* text, wchar_t* parent_item_text = NULL);//level is zero-based. if parent_item_text is NULL, then root item is inserted
	void InsertItemA(char* text, char* parent_item_text = NULL);//level is zero-based. if parent_item_text is NULL, then root item is inserted
};
