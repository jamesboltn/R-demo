#pragma once
#include"MemoryMappedFile.h"

GuruLib::MemoryMappedFile::MemoryMappedFile(wchar_t* _mapped_file_name)
{
	mapped_file_name = _mapped_file_name;
}

LPVOID GuruLib::MemoryMappedFile::CreateMappedFile(unsigned long number_of_bytes)
{
	mapped_file = ::CreateFileMapping(
		INVALID_HANDLE_VALUE, // system paging file
		NULL, // security attributes
		PAGE_READWRITE, // protection
		0, // high-order DWORD of size
		number_of_bytes, // low-order DWORD of size
		mapped_file_name.c_str()); // name

	address_of_mapped_file = ::MapViewOfFile(
		mapped_file, // handle to file-mapping object
		FILE_MAP_ALL_ACCESS, // desired access
		0,
		0,
		0);
	return address_of_mapped_file;
}

LPVOID GuruLib::MemoryMappedFile::OpenMappedFile()
{
	mapped_file = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, // read/write access
		FALSE, // do not inherit the name
		mapped_file_name.c_str()); // name of mapping object

	address_of_mapped_file = ::MapViewOfFile(
		mapped_file, // handle to file-mapping object
		FILE_MAP_ALL_ACCESS, // desired access
		0,
		0,
		0);
	return address_of_mapped_file;
}
