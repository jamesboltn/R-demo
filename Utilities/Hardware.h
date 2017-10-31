#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace Hardware
	{
		LIB void GetMachineGuid(std::wstring &guid);//This function requires administrative priviliges
		namespace CPU
		{
			LIB unsigned long GetCoreCount();
			LIB void GetModelName(std::string &model_name);
		}
		namespace Baseboard
		{
			LIB void GetSerialNumber(std::string &serial_number);
		}
		namespace Memory
		{
			LIB unsigned long long GetSizeInKiloBytes();
		}
	}
}
