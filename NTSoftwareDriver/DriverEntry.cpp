#include"Driver.h"
#include"RegistryFilter.h"
#include"ProcessFilter.h"



//#pragma INITCODE
extern "C" NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT pdriver_object,
	IN PUNICODE_STRING pregistry_path)
{
	NTSTATUS status;

	//register callbacks which will later be called by IO manager
	pdriver_object->DriverUnload = OnDriverUnload;

	for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i)
	{
		//pdriver_object->MajorFunction[IRP_MJ_CREATE] = DispatchRoutine;
		//pdriver_object->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutine;
		//pdriver_object->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
		//pdriver_object->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
		pdriver_object->MajorFunction[i] = DispatchRoutine;
	}
	

	//create driver device object
	status = CreateDevice(pdriver_object);
	if (!NT_SUCCESS(status)){ return status; }

	//create registry filter driver
	status = RegisterRegistryManipulateCallbackRoutine();
	if (!NT_SUCCESS(status)) { return status; }

	//create process filter driver
	status = RegisterProcessManipulateCallbackRoutine();
	if (!NT_SUCCESS(status)) { return status; }

	return status;
}