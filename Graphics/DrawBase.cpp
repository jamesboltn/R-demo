#pragma once
#include"DrawBase.h"

GuruLib::DrawBase::DrawBase(HWND _window_to_draw):
	window_to_draw(_window_to_draw),
	device_context(::GetDC(_window_to_draw))
{
}
GuruLib::DrawBase::~DrawBase()
{
	::ReleaseDC(window_to_draw, device_context);
}