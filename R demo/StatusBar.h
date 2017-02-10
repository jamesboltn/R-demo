#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

typedef int WidthPercent;
class StatusBar :
	public UIBase,
	public ControlBase
{
public:
	StatusBar(std::vector<WidthPercent>* width_percents);
	StatusBar() {}

	//zero based index
	void SetBlockText(unsigned int block_index, wchar_t* text);
	void SetBlockTextA(unsigned int block_index, char* text);
	void AutoSize();
};
