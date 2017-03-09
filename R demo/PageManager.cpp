#pragma once
#include"PageManager.h"
#include"UIBase.h"

void PageManager::AddPage(char* page_name, int control_count, ...)
{
	std::vector<HWND> controls;
	pages[page_name] = controls;

	// We access the ellipsis through a va_list, so let's declare one
	va_list list;

	// We initialize the va_list using va_start.  The first parameter is
	// the list to initialize.  The second parameter is the last non-ellipsis
	// parameter.
	va_start(list, control_count);

	// Loop through all the ellipsis arguments
	UIBase* uibase_ptr;
	for (unsigned int arg = 0; arg < control_count; ++arg)
	{
		// We use va_arg to get parameters out of our ellipsis
		// The first parameter is the va_list we're using
		// The second parameter is the type of the parameter
		uibase_ptr = va_arg(list, UIBase*);
		uibase_ptr->Hide();
		pages[page_name].push_back(uibase_ptr->GetHWND());
	}
	// Cleanup the va_list when we're done.
	va_end(list);
}

void PageManager::ShowPage(const char* page_name)
{
	if (curr_page == page_name)return;

	auto intended_page = pages.find(page_name);
	if (intended_page != pages.end())
	{
		//hide current page
		auto current_page = pages.find(curr_page);
		if (current_page != pages.end())
		{
			for (auto it1 = current_page->second.begin(); it1 != current_page->second.end(); ++it1)
			{
				::ShowWindowAsync(*it1, SW_HIDE);
			}
		}
		//show intended page
		for (auto it2 = intended_page->second.begin(); it2 != intended_page->second.end(); ++it2)
		{
			::ShowWindowAsync(*it2, SW_SHOW);
		}
	}
	curr_page = page_name;
}