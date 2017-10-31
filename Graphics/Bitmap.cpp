#pragma once
#include"Bitmap.h"
#include"UIBase.h"

GuruLib::Bitmap::~Bitmap()
{
	::DeleteObject(h_bitmap);
}
void GuruLib::Bitmap::SetBitmap(HBITMAP _h_bitmap)
{
	h_bitmap = _h_bitmap;
}
HBITMAP GuruLib::Bitmap::GetBitmap()
{
	return h_bitmap;
}
void GuruLib::Bitmap::CreateScreenShotAsBitmap(Bitmap &bitmap)
{
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
	SelectObject(hCaptureDC, hBitmap);
	BitBlt(hCaptureDC, 0, 0, nScreenWidth, nScreenHeight, hDesktopDC, 0, 0, SRCCOPY | CAPTUREBLT);
	ReleaseDC(hDesktopWnd, hDesktopDC);
	DeleteDC(hCaptureDC);
	bitmap.SetBitmap(hBitmap);
}
void GuruLib::Bitmap::Draw(UIBase* window)
{
	PAINTSTRUCT     ps;
	HDC             hdc;
	//BITMAP          bitmap;
	//HDC             hdcMem;
	//HGDIOBJ         oldBitmap;

	hdc = BeginPaint(window->GetHWND(), &ps);

	//hdcMem = CreateCompatibleDC(hdc);
	//oldBitmap = SelectObject(hdcMem, h_bitmap);

	//GetObject(h_bitmap, sizeof(bitmap), &bitmap);
	//BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	//SelectObject(hdcMem, oldBitmap);
	//DeleteDC(hdcMem);

	HDC       hDCBits;
	BITMAP    Bitmap;
	BOOL      bResult;

	if (!hdc || !h_bitmap)
		return;

	hDCBits = CreateCompatibleDC(hdc);
	GetObject(h_bitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	SelectObject(hDCBits, h_bitmap);
	// Replace with StretchBlt call
	//bResult = BitBlt(hDC, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hDCBits, 0, 0, dwROP);
	::SetStretchBltMode(hdc, HALFTONE);
	bResult = StretchBlt(hdc, 0, 0, window->GetWidth(), window->GetHeight(),
		hDCBits, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
	DeleteDC(hDCBits);

	EndPaint(window->GetHWND(), &ps);
}