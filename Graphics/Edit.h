#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	typedef enum
	{
		HAS_SCROLLBAR,
		NO_SCROLLBAR,
		MULTI_LINE,
		SINGLE_LINE
	}EditScrollStyle, EditLineStyle;

	class Edit :
		public UIBase,
		public ControlBase
	{
	private:
	public:
		LIB Edit();
		LIB virtual ~Edit() {}
		LIB Edit(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, EditScrollStyle scroll_style = NO_SCROLLBAR, EditLineStyle line_style = MULTI_LINE, const char* _unique_name = NULL);
		LIB Edit(const RECT &rect, UnitType type = PIXEL, EditScrollStyle scroll_style = NO_SCROLLBAR, EditLineStyle line_style = MULTI_LINE, const char* _unique_name = NULL);
		LIB explicit Edit(HWND _hwnd);//accepts HWND of created Edit

		LIB void SetReadOnly(bool is_read_only);
		LIB void SetMask(const wchar_t* mask);
		LIB void GetMask(wchar_t* mask, int length);
		//append text to the end of edit without creating a new line
		LIB void AppendText(const wchar_t* text);

		//append text to a newly created line at the end of edit
		LIB void AppendLine(const wchar_t* text);
		LIB void AppendLineA(const char* text, bool include_timestamp = true);
	};

}