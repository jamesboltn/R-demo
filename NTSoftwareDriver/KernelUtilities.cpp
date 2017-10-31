#pragma once
#include"KernelUtilities.h"

LPSTR GetProcessNameByID(HANDLE pid)
{
	PEPROCESS Process;
	if (PsLookupProcessByProcessId(pid, &Process) == STATUS_INVALID_PARAMETER)
	{
		return NULL;
	}

	// Declaration
	typedef PCHAR (*GET_PROCESS_IMAGE_NAME) (PEPROCESS Process);
	GET_PROCESS_IMAGE_NAME gGetProcessImageFileName;

	UNICODE_STRING sPsGetProcessImageFileName = RTL_CONSTANT_STRING(
		L"PsGetProcessImageFileName");
	gGetProcessImageFileName = (GET_PROCESS_IMAGE_NAME)
		MmGetSystemRoutineAddress(&sPsGetProcessImageFileName);

	if (gGetProcessImageFileName == NULL)
	{
		return NULL;
	}

	__try
	{
		return (LPSTR)(*gGetProcessImageFileName)(Process);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return NULL;
	}
}