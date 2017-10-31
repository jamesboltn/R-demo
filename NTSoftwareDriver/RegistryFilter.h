#pragma once
#include"PreInclude.h"

EX_CALLBACK_FUNCTION RegistryManipulateCallback;

#pragma INITCODE
NTSTATUS RegisterRegistryManipulateCallbackRoutine();

#pragma PAGEDCODE
void UnregisterRegistryManipulateCallbackRoutine();
