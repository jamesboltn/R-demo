#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"ComponentObjectModel.h"

namespace GuruLib
{
	class WindowsManagementInstrumentation :
		public ComponentObjectModel
	{
	private:
		bool wmi_initialized = false;
		IWbemLocator *i_wbem_locator = 0;
		IWbemServices *i_wbem_services = 0;
	public:
		LIB WindowsManagementInstrumentation();
		LIB ~WindowsManagementInstrumentation();

	};
}
