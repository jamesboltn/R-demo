#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

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
	Edit();
	virtual ~Edit() {}
	Edit(int x_pos, int y_pos, int width, int height, EditLineStyle line_style = MULTI_LINE, EditScrollStyle scroll_style = NO_SCROLLBAR, UnitType type = PIXEL, char* _unique_name = NULL);
	Edit(HWND _hwnd);//accepts HWND of created Edit
	void SetReadOnly(bool is_read_only);
	void SetMask(wchar_t* mask);
	void GetMask(wchar_t* mask, int length);
	//append text to the end of edit without creating a new line
	void AppendText(wchar_t* text);

	//append text to a newly created line at the end of edit
	void AppendLine(wchar_t* text);
	void AppendLineA(const char* text, bool include_timestamp = true);
};

class UnsignedFloatEdit :
	public Edit
{
private:
public:
	UnsignedFloatEdit() {}
	UnsignedFloatEdit(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
	UnsignedFloatEdit(HWND _hwnd);//accepts HWND of created UnsignedFloatEdit
};

class DigitalEdit :
	public Edit
{
public:
	DigitalEdit() {}
	DigitalEdit(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
	DigitalEdit(HWND _hwnd);//accepts HWND of created DigitalEdit
};


class EditWithLog :
	public Edit
{
public:
	std::string log_path;
	FILE* fp;
	EditWithLog(){}
	virtual ~EditWithLog() {};
	EditWithLog(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
	EditWithLog(HWND _hwnd);//accepts HWND of created EditWithLog
	//write msg to edit and local disk successively
	void WriteLog(const char* msg, const char* folder_path = NULL);
	//write msg to local disk only
	void WriteLogLocal(const char* msg, const char* folder_path = NULL);
};
