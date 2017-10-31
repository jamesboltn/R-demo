#pragma once
#include"RegistryFilter.h"

static LARGE_INTEGER cookies;
static BOOLEAN callback_registered = FALSE;
static UNICODE_STRING tmservice_keypath = RTL_CONSTANT_STRING(L"\\REGISTRY\\MACHINE\\SOFTWARE\\TMService");

#pragma INITCODE
NTSTATUS RegisterRegistryManipulateCallbackRoutine()
{
	//TODO:on Windows 8 and later systems,should use CmRegisterCallbackEx
	auto ret = CmRegisterCallback(RegistryManipulateCallback, NULL, &cookies);
	if (NT_SUCCESS(ret))
	{
		callback_registered = TRUE;
	}
	return ret;
}

#pragma PAGEDCODE
void UnregisterRegistryManipulateCallbackRoutine()
{
	if (callback_registered)
	{
		::CmUnRegisterCallback(cookies);
		callback_registered = FALSE;
	}
}

_Use_decl_annotations_
NTSTATUS RegistryManipulateCallback(PVOID  CallbackContext, PVOID  Argument1, PVOID  Argument2)
{
	NTSTATUS status = STATUS_SUCCESS;

	switch ((ULONG)Argument1)
	{
	case RegNtPreDeleteKey://TODO:on Windows 8 and later systems, should use CmCallbackGetKeyObjectIDEx
	{
		PREG_DELETE_KEY_INFORMATION del_key_info = (PREG_DELETE_KEY_INFORMATION)Argument2;
		if (del_key_info != NULL)
		{
			PCUNICODE_STRING reg_key_full_path;
			CmCallbackGetKeyObjectID(&cookies, del_key_info->Object, NULL, &reg_key_full_path);
			if (reg_key_full_path != NULL)
			{
				if(RtlEqualUnicodeString(reg_key_full_path, &tmservice_keypath, FALSE))
				{
					status = STATUS_CALLBACK_BYPASS;
				}
			}
		}
	}
	break;
	case RegNtPreDeleteValueKey://TODO:on Windows 8 and later systems,should use CmCallbackGetKeyObjectIDEx
	{
		PREG_DELETE_VALUE_KEY_INFORMATION del_value_info = (PREG_DELETE_VALUE_KEY_INFORMATION)Argument2;
		if (del_value_info != NULL)
		{
			PCUNICODE_STRING reg_key_full_path;
			CmCallbackGetKeyObjectID(&cookies, del_value_info->Object, NULL, &reg_key_full_path);
			if (reg_key_full_path != NULL)
			{
				if (RtlEqualUnicodeString(reg_key_full_path, &tmservice_keypath, FALSE))
				{
					status = STATUS_CALLBACK_BYPASS;
				}
			}
		}
	}
	break;
	case RegNtPreRenameKey://TODO:on Windows 8 and later systems,should use CmCallbackGetKeyObjectIDEx
	{
		PREG_RENAME_KEY_INFORMATION rename_key_info = (PREG_RENAME_KEY_INFORMATION)Argument2;
		if (rename_key_info != NULL)
		{
			PCUNICODE_STRING reg_key_full_path;
			CmCallbackGetKeyObjectID(&cookies, rename_key_info->Object, NULL, &reg_key_full_path);
			if (reg_key_full_path != NULL)
			{
				if(RtlEqualUnicodeString(reg_key_full_path, &tmservice_keypath, FALSE))
				{
					status = STATUS_CALLBACK_BYPASS;
				}
			}
		}
	}
	break;
	default:
		status = STATUS_SUCCESS;
		break;
	}
	return status;
}


