#pragma once
#include"PreInclude.h"

namespace GuruLib
{
	//note that only those objects accessed by multiple source files should be declared in the global namespace, otherwise declare them in a lower scope
	namespace NetworkGlobal
	{
#ifndef STATIC_LINK
		extern bool wsa_startup;
#endif
	}
}
