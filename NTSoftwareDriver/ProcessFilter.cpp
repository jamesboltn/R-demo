#pragma once
#include"KernelUtilities.h"
#include"ProcessFilter.h"

static PVOID registration_handle = NULL;
static bool guard_tmagentd1 = true;
static bool guard_tmdispatcher = true;
static bool guard_winproc1 = true;

#pragma INITCODE
NTSTATUS RegisterProcessManipulateCallbackRoutine()
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	UNICODE_STRING Altitude;
	USHORT filterVersion = ObGetFilterVersion();
	USHORT registrationCount = 1;
	OB_OPERATION_REGISTRATION RegisterOperation;
	OB_CALLBACK_REGISTRATION RegisterCallBack;
	REG_CONTEXT RegistrationContext;
	
	memset(&RegisterOperation, 0, sizeof(OB_OPERATION_REGISTRATION));
	memset(&RegisterCallBack, 0, sizeof(OB_CALLBACK_REGISTRATION));
	memset(&RegistrationContext, 0, sizeof(REG_CONTEXT));
	
	RegistrationContext.ulIndex = 1;
	RegistrationContext.Version = 120;
	
	if (filterVersion == OB_FLT_REGISTRATION_VERSION) 
	{
		//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Filter Version is correct.\n");
		RegisterOperation.ObjectType = PsProcessType;
		RegisterOperation.Operations = OB_OPERATION_HANDLE_CREATE;
		RegisterOperation.PreOperation = PreCallback;
		RegisterOperation.PostOperation = PostCallback;
		RegisterCallBack.Version = OB_FLT_REGISTRATION_VERSION;
		RegisterCallBack.OperationRegistrationCount = registrationCount;
		RtlInitUnicodeString(&Altitude, L"107548");
		RegisterCallBack.Altitude = Altitude;
		RegisterCallBack.RegistrationContext = &RegistrationContext;
		RegisterCallBack.OperationRegistration = &RegisterOperation;
		//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Register Callback Function Entry.\n");

		ntStatus = ObRegisterCallbacks(&RegisterCallBack, &registration_handle);
		if (ntStatus == STATUS_SUCCESS) 
		{
		}
		else 
		{
			if (ntStatus == STATUS_FLT_INSTANCE_ALTITUDE_COLLISION) 
			{
				//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Status Filter Instance Altitude Collision.\n");
			}
			if (ntStatus == STATUS_INVALID_PARAMETER) 
			{
				//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Status Invalid Parameter.\n");
			}
			if (ntStatus == STATUS_ACCESS_DENIED) 
			{
				//KdBreakPoint();
				//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "The callback routines do not reside in a signed kernel binary image.\n");
			}
			if (ntStatus == STATUS_INSUFFICIENT_RESOURCES) 
			{
				//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Status Allocate Memory Failed.\n");
			}
			//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Register Callback Function Failed with 0x%08x\n", ntStatus);
		}
		return ntStatus;
	}
	else 
	{
		//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Filter Version is not supported.\n");
		return STATUS_UNSUCCESSFUL;
	}
}

#pragma PAGEDCODE
void UnregisterProcessManipulateCallbackRoutine()
{
	if (registration_handle)
	{
		::ObUnRegisterCallbacks(registration_handle);
		registration_handle = NULL;
	}
}

VOID PostCallback(
	IN  PVOID RegistrationContext,
	IN  POB_POST_OPERATION_INFORMATION OperationInformation
)
{
	//DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "PostProcCreateRoutine. \n");
	return;
}

OB_PREOP_CALLBACK_STATUS PreCallback(
	IN  PVOID RegistrationContext,
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation
)
{
	LPSTR ProcName;
	// OB_PRE_OPERATION_INFORMATION OpInfo;

	UNREFERENCED_PARAMETER(RegistrationContext);

	ProcName = GetProcessNameByID(PsGetProcessId((PEPROCESS)OperationInformation->Object));

	if (strcmp(ProcName, "winproc1.exe") == 0)
	{
		if (guard_winproc1)
		{
			ProctectProcess(OperationInformation);
		}
	}
	else if (strcmp(ProcName, "tmagentd1.exe") == 0)
	{
		if (guard_tmagentd1)
		{
			ProctectProcess(OperationInformation);
		}
	}
	else if (strcmp(ProcName, "tmdispatcher.exe") == 0)
	{
		if (guard_tmdispatcher)
		{
			ProctectProcess(OperationInformation);
		}
	}

	return OB_PREOP_SUCCESS;
}

void ProctectProcess(
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation
)
{
	if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
	{
		if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
		}
		if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_OPERATION) == PROCESS_VM_OPERATION)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_OPERATION;
		}
		if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & ~PROCESS_VM_READ) == PROCESS_VM_READ)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
		}
		if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_WRITE) == PROCESS_VM_WRITE)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_WRITE;
		}
	}
}
