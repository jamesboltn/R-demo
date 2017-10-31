#pragma once
#include"Page.h"
#include"UIBase.h"

void GuruLib::Page::RegisterRenderedControls(unsigned short control_count, ...)
{
	// We access the ellipsis through a va_list, so let's declare one
	va_list list;

	// We initialize the va_list using va_start.  The first parameter is
	// the list to initialize.  The second parameter is the last non-ellipsis
	// parameter.
	va_start(list, control_count);

	// Loop through all the ellipsis 
	UIBase* ui_base_ptr;
	for (unsigned int arg = 0; arg < control_count; ++arg)
	{
		// We use va_arg to get parameters out of our ellipsis
		// The first parameter is the va_list we're using
		// The second parameter is the type of the parameter
		ui_base_ptr = va_arg(list, UIBase*);
		if (ui_base_ptr != NULL)
		{
			controls.push_back(ui_base_ptr);
		}
	}
	// Cleanup the va_list when we're done.
	va_end(list);
}
std::vector<GuruLib::UIBase*>& GuruLib::Page::GetRenderedControls()
{
	return controls;
}