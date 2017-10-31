#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class ControlBase
	{
	private:
		static int _resource_id;
	protected:
		void IncrementResourceID();
	public:
		int resource_id;
		std::string unique_name;
#ifdef DLL_EXPORT
		LIB
#endif
			ControlBase();
		virtual ~ControlBase();
	};

}
