#pragma once
#include"PreInclude.h"

namespace GuruLib
{
	class DrawBase
	{
	private:
		HDC device_context;
		HWND window_to_draw;
	protected:

	public:
		explicit DrawBase(HWND _window_to_draw);
		~DrawBase();
	};
}

