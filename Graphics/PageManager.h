#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	typedef std::string PageName;

	class UIBase;
	class Page;
	class PageManager
	{
	private:
		std::unordered_map<PageName, std::vector<HWND>> page_hwnds;
		std::unordered_map<PageName, Page*> pages;
		std::string curr_page;
	protected:

	public:
		LIB void AddPages(unsigned short page_count, ...);
		LIB void ShowPage(const char* page_name);
	};
}

