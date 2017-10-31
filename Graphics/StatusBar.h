#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	typedef unsigned short WidthPercent;
	class StatusBar :
		public UIBase,
		public ControlBase
	{
	public:
		LIB explicit StatusBar(const std::vector<WidthPercent> &width_percents);
		LIB StatusBar() {}

		//zero based index
		LIB void SetBlockText(unsigned int block_index, const wchar_t* text);
		LIB void SetBlockTextA(unsigned int block_index, const char* text);
		LIB void AutoSize();
	};
}

