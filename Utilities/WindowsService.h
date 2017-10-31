#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
#define KERNEL_DRIVER_SERVICE 0
#define USER_MODE_SERVICE     1
	typedef char ServiceType;

	class WindowsService
	{
	private/*static*/:
	protected/*static*/:
	public/*static*/:
		static bool InstallService(wchar_t* service_binary_path, wchar_t* service_name);
		static bool DeleteService(wchar_t* service_name);
		static bool ServiceExist(wchar_t* service_name);
		LIB static BOOL LaunchApplicationFromUserSession(const wchar_t* module_path, wchar_t* command_line = NULL);
	private:
		SERVICE_STATUS service_status;
		SC_HANDLE handle_scm = OpenSCManager(0, 0, GENERIC_READ | SC_MANAGER_ALL_ACCESS);
		SERVICE_STATUS_HANDLE service_status_handle = NULL;
		HANDLE stop_event = INVALID_HANDLE_VALUE;
		DWORD current_status;
		std::wstring service_name;
	protected:
	public:
		LIB explicit WindowsService(const wchar_t* _servie_name);
		LIB WindowsService();
		LIB ~WindowsService();
		LIB wchar_t* GetName() { return const_cast<wchar_t*>(service_name.c_str()); }
		LIB void SetName(const wchar_t* _service_name) { service_name = _service_name; }
		LIB void SetServiceStatusHandle(SERVICE_STATUS_HANDLE srv_status_handle) { service_status_handle = srv_status_handle; }
		LIB SERVICE_STATUS_HANDLE GetServiceStatusHandle() { return service_status_handle; }
		LIB BOOL ReportStatus(unsigned long status);
		LIB BOOL ReportStatus();
		LIB SERVICE_STATUS* GetServiceStatus() { return &service_status; }
		LIB void SetStopEvent(HANDLE hdl) { stop_event = hdl; }
		LIB HANDLE GetStopEvent() { return stop_event; }
		LIB void ResetName(wchar_t* new_name) { service_name = new_name; }
		LIB bool Delete(const wchar_t* _service_name);//Note in order to delete a service, you should first Stop();
		LIB bool Delete();//Note in order to delete a service, you should first Stop();
		LIB bool Install(const wchar_t* service_binary_path, ServiceType service_type = USER_MODE_SERVICE);//note the service binary path should be full qualified
		LIB bool Stop();
		LIB bool Start();//Note in order to start a service, you should first Install()
		LIB bool Exist();//check if the service of which the name in constructor exists
		LIB bool Running();//check if the service of which the name in constructor is running
		LIB bool Enable();
		LIB bool Disable();
	};
}


