#pragma once
#include"Hardware.h"
#include"Registry.h"
#include"Utilities.h"
#include<atlconv.h>

void GuruLib::Hardware::GetMachineGuid(std::wstring &guid)
{
	GuruLib::Registry reg(HKEY_LOCAL_MACHINE);
	if (reg.SetKey(L"SOFTWARE\\Microsoft\\Cryptography"))
	{
		reg.ReadString(L"MachineGuid", L"", guid);
	}
}

unsigned long GuruLib::Hardware::CPU::GetCoreCount()
{
	SYSTEM_INFO sysinfo;
	::GetNativeSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

void GuruLib::Hardware::CPU::GetModelName(std::string &model_name)
{
	int CPUInfo[4] = { -1 };
	char CPUBrandString[0x40];
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	memset(CPUBrandString, 0, GuruLib::ArraySize(CPUBrandString));

	// Get the information associated with each extended ID.
	for (int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string.
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	model_name = CPUBrandString;
}

unsigned long long GuruLib::Hardware::Memory::GetSizeInKiloBytes()
{
	unsigned long long size;
	if(!::GetPhysicallyInstalledSystemMemory(&size))
	{
		return 0;
	}
	return size;
}


#ifndef MACRO_T_DEVICE_PROPERTY  
#define MACRO_T_DEVICE_PROPERTY  

#define PROPERTY_MAX_LEN    128 // 属性字段最大长度  
typedef struct _T_DEVICE_PROPERTY
{
	TCHAR szProperty[PROPERTY_MAX_LEN];
} T_DEVICE_PROPERTY;
#endif  

#define WMI_QUERY_TYPENUM   7   // WMI查询支持的类型数  
#pragma comment (lib, "comsuppw.lib")  
#pragma comment (lib, "wbemuuid.lib")  

typedef struct _T_WQL_QUERY
{
	CHAR*   szSelect;       // SELECT语句  
	WCHAR*  szProperty;     // 属性字段  
} T_WQL_QUERY;

// WQL查询语句  
const T_WQL_QUERY szWQLQuery[] = {
	// 网卡原生MAC地址  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"PNPDeviceID",

	// 硬盘序列号  
	"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')",
	L"SerialNumber",

	// 主板序列号  
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	// 处理器ID  
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)",
	L"ProcessorId",

	// BIOS序列号  
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	// 主板型号  
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)",
	L"Product",

	// 网卡当前MAC地址  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"MACAddress",
};


static BOOL WMI_DoWithProperty(INT iQueryType, TCHAR *szProperty, UINT uSize)
{
	BOOL isOK = TRUE;

	switch (iQueryType)
	{
	case 6:     // 网卡当前MAC地址  
				// 去掉冒号  
		std::remove(szProperty, szProperty + _tcslen(szProperty) + 1, L':');
		break;
	default:
		// 去掉空格  
		std::remove(szProperty, szProperty + _tcslen(szProperty) + 1, L' ');
	}

	return isOK;
}

// 基于Windows Management Instrumentation（Windows管理规范）  
INT WMI_DeviceQuery(INT iQueryType, T_DEVICE_PROPERTY *properties, INT iSize)
{
	HRESULT hres;
	INT iTotal = 0;

	// 判断查询类型是否支持  
	if ((iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery) / sizeof(T_WQL_QUERY)))
	{
		return -1;  // 查询类型不支持  
	}

	// 初始化COM  
	hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		return -2;
	}

	// 设置COM的安全认证级别  
	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);
	if (FAILED(hres))
	{
		CoUninitialize();
		return -2;
	}

	// 获得WMI连接COM接口  
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&pLoc)
	);
	if (FAILED(hres))
	{
		CoUninitialize();
		return -2;
	}

	// 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"  
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		NULL,
		0,
		NULL,
		NULL,
		&pSvc
	);
	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// 设置请求代理的安全级别  
	hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// 通过请求代理来向WMI发送请求  
	IEnumWbemClassObject *pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(szWQLQuery[iQueryType].szSelect),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -3;
	}

	// 循环枚举所有的结果对象    
	while (pEnumerator)
	{
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		if ((properties != NULL) && (iTotal >= iSize))
		{
			break;
		}

		pEnumerator->Next(
			WBEM_INFINITE,
			1,
			&pclsObj,
			&uReturn
		);

		if (uReturn == 0)
		{
			break;
		}

		if (properties != NULL)
		{   // 获取属性值  
			VARIANT vtProperty;

			VariantInit(&vtProperty);
			pclsObj->Get(szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL);
			USES_CONVERSION; StringCchCopy(properties[iTotal].szProperty, PROPERTY_MAX_LEN, W2T(vtProperty.bstrVal));
			VariantClear(&vtProperty);

			// 对属性值做进一步的处理  
			if (WMI_DoWithProperty(iQueryType, properties[iTotal].szProperty, PROPERTY_MAX_LEN))
			{
				iTotal++;
			}
		}
		else
		{
			iTotal++;
		}

		pclsObj->Release();
	} // End While  

	  // 释放资源  
	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

	return iTotal;
}
void GuruLib::Hardware::Baseboard::GetSerialNumber(std::string &serial_number)
{
#pragma comment (lib, "comsuppw.lib")  
#pragma comment (lib, "wbemuuid.lib")  

	T_DEVICE_PROPERTY mac;
	WMI_DeviceQuery(2, &mac, 2);

	std::wstring serial_number_w = mac.szProperty;
	TypeConverter::WStringToString(serial_number_w, serial_number);

}
