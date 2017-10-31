#pragma once
#include"PageManager.h"
#include"Page.h"
#include"UIBase.h"

void GuruLib::PageManager::AddPages(unsigned short page_count, ...)
{
	// We access the ellipsis through a va_list, so let's declare one
	va_list list;

	// We initialize the va_list using va_start.  The first parameter is
	// the list to initialize.  The second parameter is the last non-ellipsis
	// parameter.
	va_start(list, page_count);

	// Loop through all the ellipsis arguments
	char* page_name;
	for (unsigned int arg = 0; arg < 2 * page_count;)
	{
		// We use va_arg to get parameters out of our ellipsis
		// The first parameter is the va_list we're using
		// The second parameter is the type of the parameter
		page_name = va_arg(list, char*);
		pages[page_name] = va_arg(list, Page*);
		arg += 2;
	}
	// Cleanup the va_list when we're done.
	va_end(list);
}
void GuruLib::PageManager::ShowPage(const char* page_name)
{
	if (curr_page == page_name)return;

	auto intended_page = page_hwnds.find(page_name);
	if (intended_page != page_hwnds.end())
	{
		//hide current page
		auto current_page = page_hwnds.find(curr_page);
		if (current_page != page_hwnds.end())
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
			if ((it2 + 1) == intended_page->second.end())
			{
				HWND parent = ::GetParent(*it2);
				if (parent != NULL)
				{
					char window_caption[100];
					strcpy_s(window_caption, ArraySize(window_caption), page_name);
					strcat_s(window_caption, ArraySize(window_caption), " - TM Service GUI (for testing purpose only)");
					::SetWindowTextA(parent, window_caption);
				}
			}
		}
		curr_page = page_name;
	}
	else 
	{
		auto to_be_created_page = pages.find(page_name);
		if (to_be_created_page != pages.end())
		{
			//hide current page
			auto current_page = page_hwnds.find(curr_page);
			if (current_page != page_hwnds.end())
			{
				for (auto it1 = current_page->second.begin(); it1 != current_page->second.end(); ++it1)
				{
					::ShowWindowAsync(*it1, SW_HIDE);
				}
			}

			//create new page
			to_be_created_page->second->Render();

			for (auto it = (to_be_created_page->second->GetRenderedControls()).begin(); it != (to_be_created_page->second->GetRenderedControls()).end(); ++it)
			{
				page_hwnds[page_name].push_back((*it)->GetHWND());

				if ((it + 1) == to_be_created_page->second->GetRenderedControls().end())
				{
					char window_caption[100];
					strcpy_s(window_caption, GuruLib::ArraySize(window_caption), page_name);
					strcat_s(window_caption, GuruLib::ArraySize(window_caption), " - TM Service GUI (for testing purpose only)");
					::SetWindowTextA((*it)->GetParentHWND(), window_caption);
				}
			}
			curr_page = page_name;
		}
	}
}