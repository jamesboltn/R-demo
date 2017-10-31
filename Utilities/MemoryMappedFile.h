#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"MemoryMappedFile.h"

namespace GuruLib
{
	class MemoryMappedFile
	{
	private:
		HANDLE mapped_file = NULL;
		LPVOID address_of_mapped_file = NULL;
		std::wstring mapped_file_name;
	protected:

	public:
		LIB explicit MemoryMappedFile(wchar_t* _mapped_file_name);
		LIB LPVOID CreateMappedFile(unsigned long number_of_bytes);
		LIB LPVOID OpenMappedFile();
	};
}

