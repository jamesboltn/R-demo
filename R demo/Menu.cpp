#pragma once
#include"Menu.h"

Menu::Menu()
{
	menu_bar = ::CreateMenu();
}

int Menu::GetResourceID(wchar_t* text)
{
	auto it = caption_resourceid_map.find(text);
	if (it == caption_resourceid_map.end())
	{
		return -1;
	}
	else
	{
		return it->second;
	}
}

void Menu::InsertTtem(wchar_t* text, wchar_t* parent_item_text)
{
	if (parent_item_text == NULL)
	{
		HMENU menu_item = ::CreateMenu();
		::AppendMenuW(menu_bar, MF_POPUP, (UINT_PTR)menu_item, text);
		caption_rootmenu_map.insert(std::make_pair(text, menu_item));
	}
	else
	{
		auto it = caption_rootmenu_map.find(parent_item_text);
		if (it == caption_rootmenu_map.end())
		{
			return;
		}
		IncrementResourceID();
		::AppendMenuW(it->second, MF_STRING, resource_id, text);
		::SetMenu(UIBase::last_created_window_hwnd, menu_bar);
		caption_resourceid_map.insert(std::make_pair(text, resource_id));
	}
}