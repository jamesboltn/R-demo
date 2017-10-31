#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"ControlBase.h"
#include"UIBase.h"
#include"GraphicsDef.h"

namespace GuruLib
{
	typedef std::wstring MenuItemCaption;
	class Menu :
		public ControlBase, public UIBase
	{
	private:
		HMENU menu_bar;
		std::unordered_map<MenuItemCaption, ResourceID> caption_resourceid_map;
		std::unordered_map<MenuItemCaption, HMENU> caption_rootmenu_map;
	public:
		LIB Menu();

		LIB void InsertTtem(const wchar_t* text, const wchar_t* parent_item_text = NULL);
		LIB int GetResourceID(const wchar_t* text);
	};
}
