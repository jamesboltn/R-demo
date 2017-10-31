#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class InstanceLimit
	{
		enum { MAX_INSTANCES = 16 };
		int instance_count;
		std::vector<std::wstring> names;
		int current_instance_index;//one_based
		HANDLE this_handle;
	public:
		LIB InstanceLimit(int limit = 1, wchar_t* name = NULL);
		LIB virtual ~InstanceLimit();
		LIB bool ExceedLimit();
		inline int GetCurrentInstanceIndex() { return current_instance_index; }
	};
}
