#pragma once
#include"ControlBase.h"
#include"preinclude.h"
#include"UIBase.h"

typedef std::wstring MenuItemCaption;
typedef int ResourceID;
class Menu:
	public ControlBase, public UIBase
{
private:
	HMENU menu_bar;
	std::unordered_map<MenuItemCaption, ResourceID> caption_resourceid_map;
	std::unordered_map<MenuItemCaption, HMENU> caption_rootmenu_map;
public:
	Menu();
	void InsertTtem(wchar_t* text, wchar_t* parent_item_text = NULL);
	int GetResourceID(wchar_t* text);
};