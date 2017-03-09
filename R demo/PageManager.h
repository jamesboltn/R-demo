#pragma once
#include"preinclude.h"

typedef std::string PageName;

class UIBase;
class PageManager
{
private:
	std::unordered_map<PageName, std::vector<HWND>> pages;
	std::string curr_page;
protected:
	void AddPage(char* page_name, int control_count, ...);
	void ShowPage(const char* page_name);
};
