#pragma once
#include"Driver.h"
#include"RegistryFilter.h"
#include"ProcessFilter.h"

//create device name
UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\NTVirtualDevice");

//create symbol link
UNICODE_STRING symbol_link_name = RTL_CONSTANT_STRING(L"\\??\\VirtualDevice");

PDEVICE_OBJECT pdevice_object = NULL;

#pragma INITCODE
NTSTATUS CreateDevice(IN PDRIVER_OBJECT pdriver_object)
{
	NTSTATUS status;
	/*pDeviceExtension pdevice_ext;*/

	//create device
	status = IoCreateDevice(pdriver_object,
		0/*sizeof(DeviceExtension)*/,
		&(UNICODE_STRING)device_name,
		FILE_DEVICE_UNKNOWN,
		0, 
		TRUE,
		&pdevice_object);
	
	if (!NT_SUCCESS(status))
	{
		return status;
	}
		
	/*pdevice_object->Flags |= DO_BUFFERED_IO;*/
	/*pdevice_ext = (pDeviceExtension)pdevice_object->DeviceExtension;*/
	/*pdevice_ext->pdevice = pdevice_object;*/
	/*pdevice_ext->device_name = device_name;*/

	/*pdevice_ext->symbol_link_name = symbol_link_name;*/
	IoDeleteSymbolicLink(&symbol_link_name);
	status = IoCreateSymbolicLink(&symbol_link_name, &device_name);
	
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pdevice_object);
		return status;
	}

	// 清除控制设备的初始化标记。
	pdevice_object->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
VOID OnDriverUnload(IN PDRIVER_OBJECT pdriver_object)
{
	//PDEVICE_OBJECT	next_device_object = pdriver_object->DeviceObject;
	//while (next_device_object != NULL)
	//{
		//pDeviceExtension pdevice_ext = (pDeviceExtension)next_device_object->DeviceExtension;

		////delete symbol link
		//UNICODE_STRING link_name = pdevice_ext->symbol_link_name;
		ASSERT(pdevice_object != NULL);
		IoDeleteSymbolicLink(&symbol_link_name);
		//next_device_object = next_device_object->NextDevice;
		IoDeleteDevice(pdevice_object);
	//}

	//unreg registry filter driver
	UnregisterRegistryManipulateCallbackRoutine();

	//unreg process filter driver
	UnregisterProcessManipulateCallbackRoutine();
}

#pragma PAGEDCODE
NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT _pdevice_object,
	IN PIRP pirp)
{
	PIO_STACK_LOCATION  curr_irp_stack_location = IoGetCurrentIrpStackLocation(pirp);
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ret_len = 0;
	if (_pdevice_object == pdevice_object)
	{
		// 如果这个请求不是发给pdevice_object的，那就非常奇怪了。
		// 因为这个驱动只生成过这一个设备。所以可以直接
		// 返回失败。
		if (curr_irp_stack_location->MajorFunction == IRP_MJ_CREATE || curr_irp_stack_location->MajorFunction == IRP_MJ_CLOSE)
		{
			// 生成和关闭请求，这个一律简单地返回成功就可以
			// 了。就是无论何时打开和关闭都可以成功。
			goto IRP_Complete;
		}

		if (curr_irp_stack_location->MajorFunction == IRP_MJ_DEVICE_CONTROL)
		{
			// 处理DeviceIoControl。
			PVOID buffer = pirp->AssociatedIrp.SystemBuffer;
			ULONG inlen = curr_irp_stack_location->Parameters.DeviceIoControl.InputBufferLength;
			ULONG outlen = curr_irp_stack_location->Parameters.DeviceIoControl.OutputBufferLength;
			ULONG len;
			switch (curr_irp_stack_location->Parameters.DeviceIoControl.IoControlCode)
			{
			case CWK_DVC_SEND_STR:
				ASSERT(buffer != NULL);
				ASSERT(inlen > 0);
				ASSERT(outlen == 0);
				DbgPrint((char *)buffer);
				// 已经打印过了，那么现在就可以认为这个请求已经成功。
				break;
			case CWK_DVC_RECV_STR:
			default:
				// 到这里的请求都是不接受的请求。未知的请求一律返回非法参数错误。
				status = STATUS_INVALID_PARAMETER;
				break;
			}
		}
	}
	
IRP_Complete:
	//IRP accomplished
	pirp->IoStatus.Status = status;
	pirp->IoStatus.Information = ret_len;	// bytes xfered
	IoCompleteRequest(pirp, IO_NO_INCREMENT);
	return status;
}