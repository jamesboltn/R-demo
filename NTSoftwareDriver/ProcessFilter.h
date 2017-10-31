#pragma once
#include"PreInclude.h"

//Process Security and Access Rights
#define PROCESS_CREATE_THREAD  (0x0002)
#define PROCESS_CREATE_PROCESS (0x0080)
#define PROCESS_TERMINATE      (0x0001)
#define PROCESS_VM_WRITE       (0x0020)
#define PROCESS_VM_READ        (0x0010)
#define PROCESS_VM_OPERATION   (0x0008)
#define PROCESS_SUSPEND_RESUME (0x0800)

typedef struct _OB_REG_CONTEXT {
	__in USHORT Version;
	__in UNICODE_STRING Altitude;
	__in USHORT ulIndex;
	OB_OPERATION_REGISTRATION *OperationRegistration;
} REG_CONTEXT, *PREG_CONTEXT;

#pragma INITCODE
NTSTATUS RegisterProcessManipulateCallbackRoutine();

#pragma PAGEDCODE
void UnregisterProcessManipulateCallbackRoutine();

OB_PREOP_CALLBACK_STATUS PreCallback(
	IN  PVOID RegistrationContext,
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation);

VOID PostCallback(
	IN  PVOID RegistrationContext,
	IN  POB_POST_OPERATION_INFORMATION OperationInformation
);

void ProctectProcess(
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation
);

