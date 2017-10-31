#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	typedef int LOW, HIGH;
	class ProgressBar :
		public UIBase,
		public ControlBase
	{
	public:
		LIB ProgressBar(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit ProgressBar(HWND _hwnd);//accepts HWND of created ProgressBar
		LIB ProgressBar() {}
		LIB virtual ~ProgressBar() {}

		LIB void SetRange(int min, int max);
		LIB std::pair<LOW, HIGH> GetRange();
	};
}
