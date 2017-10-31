#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class UIBase;
	class Bitmap
	{
	private:
		HBITMAP h_bitmap;
	protected:

	public:
		LIB ~Bitmap();

		LIB void SetBitmap(HBITMAP _h_bitmap);
		LIB HBITMAP GetBitmap();
		LIB void Draw(UIBase* window);//draw bitmap stretchly on the window

		LIB static void CreateScreenShotAsBitmap(Bitmap &bitmap);
	};
}
