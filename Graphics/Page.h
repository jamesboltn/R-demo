#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"ControlMessageHandler.h"
#include"UIBase.h"
#include"Window.h"

namespace GuruLib
{
	class PageManager;
	class UIBase;
	class Page :
		public ControlMessageHandler
	{
	private:
		std::vector<UIBase*> controls;
	protected:
		LIB void RegisterRenderedControls(unsigned short control_count, ...);
	public:
		virtual void Render() = 0;
		std::vector<UIBase*>& GetRenderedControls();
	};
}


