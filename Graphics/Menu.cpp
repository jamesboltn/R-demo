#pragma once
#include"Menu.h"

GuruLib::Menu::Menu()
{
	menu_bar = ::CreateMenu();
}
int GuruLib::Menu::GetResourceID(const wchar_t* text)
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
void GuruLib::Menu::InsertTtem(const wchar_t* text, const wchar_t* parent_item_text)
{
	if (parent_item_text == NULL)
	{
		HMENU menu_item = ::CreateMenu();
		::AppendMenuW(menu_bar, MF_POPUP, (UINT_PTR)menu_item, text);
		caption_rootmenu_map.insert({ text, menu_item });
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
		caption_resourceid_map.insert({ text, resource_id });
	}
}