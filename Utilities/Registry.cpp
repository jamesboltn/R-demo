#pragma once
#include"Registry.h"
#include"System.h"

#define CLASS_NAME_LENGTH 255

/* IMPORTANT NOTES ABOUT CLASS REGISTRY:
	
	Registry never keeps a key open past the end of a function call.
	This is incase the application crashes before the next call to close
	the registry 

	KEY NAMES:
	Key names must not begin with a \ and only absolute strings are accepted	
*/

GuruLib::Registry::Registry(HKEY hRootKey)
{
	is_wow64_process = System::Process::IsWow64Process();

	m_hRootKey = hRootKey;
	m_bLazyWrite = TRUE;
	m_nLastError = ERROR_SUCCESS;
}

GuruLib::Registry::~Registry()
{
	ClearKey();
}

void GuruLib::Registry::UseWow64KeyPath(bool flag)
{
	//use Wow64 key path only when this function is executed in a Wow64 process
	if (System::Process::IsWow64Process())
	{
		if (flag)
		{
			is_wow64_process = FALSE;
		}
		else
		{
			is_wow64_process = TRUE;
		}
	}
}

BOOL GuruLib::Registry::ClearKey()
{
	/* Call CloseKey to write the current key to the registry and close the 
	key. An application should not keep keys open any longer than necessary. 
	Calling CloseKey when there is no current key has no effect.*/

	m_strCurrentPath.clear();
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	return TRUE;
}

BOOL GuruLib::Registry::SetRootKey(HKEY hRootKey)
{
	// sets the root key
	// make sure to set it to a valid key
	if (hRootKey != HKEY_CLASSES_ROOT &&
			hRootKey != HKEY_CURRENT_USER &&
			hRootKey != HKEY_LOCAL_MACHINE &&
			hRootKey != HKEY_USERS) return FALSE;

	m_hRootKey = hRootKey;
	return TRUE;
}

BOOL GuruLib::Registry::CreateKey(wchar_t* strKey)
{
	/* Use CreateKey to add a new key to the registry. 
		Key is the name of the key to create. Key must be 
		an absolute name. An absolute key 
		begins with a backslash (\) and is a subkey of 
		the root key. */

	assert(strKey[0] != '\\');
	HKEY hKey;

	DWORD dwDisposition = 0;

	if (::RegCreateKeyEx(m_hRootKey, strKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, is_wow64_process? KEY_ALL_ACCESS| KEY_WOW64_64KEY :KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition)	!= ERROR_SUCCESS) return FALSE;
	
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	m_strCurrentPath = strKey;
	return TRUE;
}

BOOL GuruLib::Registry::DeleteKey()
{
	/* Call DeleteKey to remove a specified key and its associated data,
	if any, from the registry. Returns FALSE is there are subkeys
	Subkeys must be explicitly deleted by separate calls to DeleteKey.
	DeleteKey returns True if key deletion is successful. On error,
	DeleteKey returns False. */

	// need to open the key first with RegOpenKeyEx
	//assert(FALSE); // not yet implemented
	assert(m_strCurrentPath.length() > 0);

	if (!KeyExists()) return TRUE;
	if (is_wow64_process)
	{
		if (::RegDeleteKeyEx(m_hRootKey, m_strCurrentPath.c_str(), KEY_WOW64_64KEY, NULL) != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
	else
	{
		if (::RegDeleteKey(m_hRootKey, m_strCurrentPath.c_str()) != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL GuruLib::Registry::DeleteKey(wchar_t* strKey)
{
	/* Call DeleteKey to remove a specified key and its associated data, 
	if any, from the registry. Returns FALSE is there are subkeys
	Subkeys must be explicitly deleted by separate calls to DeleteKey.
	DeleteKey returns True if key deletion is successful. On error, 
	DeleteKey returns False. */
	
	// need to open the key first with RegOpenKeyEx
	//assert(FALSE); // not yet implemented
	assert(strKey[0] != '\\');

	if (!KeyExists(strKey)) return TRUE;
	if (is_wow64_process)
	{
		if (::RegDeleteKeyEx(m_hRootKey, strKey, KEY_WOW64_64KEY, NULL) != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
	else
	{
		if (::RegDeleteKey(m_hRootKey, strKey) != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL GuruLib::Registry::DeleteValue(wchar_t* strName)
{
	/* Call DeleteValue to remove a specific data value 
		associated with the current key. Name is string 
		containing the name of the value to delete. Keys can contain 
		multiple data values, and every value associated with a key 
		has a unique name. */

	assert(m_strCurrentPath.length() > 0);
	HKEY hKey;
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_SET_VALUE| KEY_WOW64_64KEY : KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegDeleteValue(hKey, LPCTSTR(strName));
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

int GuruLib::Registry::GetDataSize(wchar_t* strValueName)
{
	/* Call GetDataSize to determine the size, in bytes, of 
	a data value associated with the current key. ValueName 
	is a string containing the name of the data value to query.
	On success, GetDataSize returns the size of the data value. 
	On failure, GetDataSize returns -1. */

	HKEY hKey;
	assert(m_strCurrentPath.length() > 0);
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_WOW64_64KEY| KEY_QUERY_VALUE : KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) return -1;

	DWORD dwSize = 1;
	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strValueName),
		NULL, NULL, NULL, &dwSize);
	::RegCloseKey(hKey);

	if (lResult != ERROR_SUCCESS) return -1;
	return (int)dwSize;
}

DWORD GuruLib::Registry::GetDataType(wchar_t* strValueName)
{
	HKEY hKey;
	assert(m_strCurrentPath.length() > 0);

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_QUERY_VALUE| KEY_WOW64_64KEY : KEY_QUERY_VALUE, &hKey);

	if (m_nLastError != ERROR_SUCCESS) return 0;

	DWORD dwType = 1;
	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strValueName),
		NULL, &dwType, NULL, NULL);
	::RegCloseKey(hKey);		

	if (m_nLastError == ERROR_SUCCESS) return dwType;

	return 0;
}

int GuruLib::Registry::GetSubKeyCount()
{
	/* Call this function to determine the number of subkeys.
		the function returns -1 on error */
	HKEY hKey;
	assert(m_strCurrentPath.length() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_WOW64_64KEY |KEY_ALL_ACCESS : KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwSubKeyCount;
}

int GuruLib::Registry::GetValueCount()
{
	/* Call this function to determine the number of subkeys.
		the function returns -1 on error */
	HKEY hKey;
	assert(m_strCurrentPath.length() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_WOW64_64KEY | KEY_ALL_ACCESS : KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwValueCount;
}

BOOL GuruLib::Registry::KeyExists(wchar_t* strKey, HKEY hRootKey)
{
	/* Call KeyExists to determine if a key of a specified name exists.
		 Key is the name of the key for which to search. */

	assert(strKey[0] != '\\');
	HKEY hKey;

	if (hRootKey == NULL) hRootKey = m_hRootKey;
	
	LONG lResult = ::RegOpenKeyEx(hRootKey, LPCTSTR(strKey), 0,
		is_wow64_process? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey);
	::RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS)
	{
		m_strCurrentPath = strKey;
		return TRUE;
	}
	return FALSE;
}

BOOL GuruLib::Registry::KeyExists()
{
	/* Call KeyExists to determine if a key of a specified name exists. Key is the name of the key for which to search. */
	assert(m_strCurrentPath.length() > 0 || m_hRootKey == NULL);

	HKEY hKey;
	LONG lResult = ::RegOpenKeyEx(m_hRootKey, m_strCurrentPath.c_str(), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey);
	::RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL GuruLib::Registry::SetKey(const wchar_t* key, bool can_create)
{
	/* Call SetKey to make a specified key the current key. Key is the 
		name of the key to open. If Key is null, the CurrentKey property 
		is set to the key specified by the RootKey property.

		CanCreate specifies whether to create the specified key if it does 
		not exist. If CanCreate is True, the key is created if necessary.

		Key is opened or created with the security access value KEY_ALL_ACCESS. 
		OpenKey only creates non-volatile keys, A non-volatile key is stored in 
		the registry and is preserved when the system is restarted. 

		OpenKey returns True if the key is successfully opened or created */

	assert(key[0] != '\\');
	
	// close the current key if it is open
	if (wcslen(key) == 0)
	{
		m_strCurrentPath.clear();
		return TRUE;
	}

	DWORD dwDisposition;
	HKEY hKey;
	if (can_create) // open the key with RegCreateKeyEx
	{
		if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(key), 0, NULL, 
			REG_OPTION_NON_VOLATILE, is_wow64_process ? KEY_WOW64_64KEY | KEY_ALL_ACCESS :KEY_ALL_ACCESS, NULL, &hKey,
				&dwDisposition) != ERROR_SUCCESS) return FALSE;
		m_strCurrentPath = key;
		if (!m_bLazyWrite) ::RegFlushKey(hKey);
		::RegCloseKey(hKey);	
		return TRUE;
	}

	// otherwise, open the key without creating
	// open key requires no initial slash
	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(key), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ:KEY_READ, &hKey);
	if (m_nLastError != ERROR_SUCCESS) return FALSE;
	m_strCurrentPath = key;
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return TRUE;
}

BOOL GuruLib::Registry::ValueExists(wchar_t* strName)
{
	/* Call ValueExists to determine if a particular key exists in 
		the registry. Calling Value Exists is especially useful before 
		calling other TRegistry methods that operate only on existing keys.

		Name is the name of the data value for which to check.
	ValueExists returns True if a match if found, False otherwise. */

	HKEY hKey;
	LONG lResult;
	assert(m_strCurrentPath.length() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_QUERY_VALUE : KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		NULL, NULL, NULL);
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

void GuruLib::Registry::RenameValue(wchar_t* strOldName, wchar_t* strNewName)
{
	/* Call RenameValue to change the name of a data value associated 
		with the current key. OldName is a string containing the current 
		name of the data value. NewName is a string containing the replacement 
		name for the data value.
		
		If OldName is the name of an existing data value for the current key, 
		and NewName is not the name of an existing data value for the current 
		key, RenameValue changes the data value name as specified. Otherwise 
		the current name remains unchanged.
	*/
	assert(FALSE); // functionality not yet implemented
}

//COleDateTime GuruLib::Registry::ReadDateTime(wchar_t* strName, COleDateTime dtDefault)
//{
//	/* Call ReadDate to read a date value from a specified data value 
//	associated with the current key. Name is the name of the data value to read.
//	If successful, ReadDate returns a Delphi TDateTime value. The integral part 
//	of a TDateTime value is the number of days that have passed since 12/30/1899. 
//	The fractional part of a TDateTime value is the time of day.
//	On error, an exception is raised, and the value returned by this function 
//	should be discarded. */
//
//	DWORD dwType = REG_BINARY;
//	COleDateTime dt;
//	DWORD dwSize = sizeof(dt);
//	HKEY hKey;
//
//	assert(m_strCurrentPath.length() > 0);
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY|KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return dtDefault;
//	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
//		&dwType, (LPBYTE)&dt, &dwSize) != ERROR_SUCCESS) dt = dtDefault;
//	::RegCloseKey(hKey);	
//	return dt;
//}

double GuruLib::Registry::ReadFloat(wchar_t* strName, double fDefault)
{
	/* Call ReadFloat to read a float value from a specified 
		data value associated with the current key. Name is the name 
		of the data value to read.
		
		If successful, ReadFloat returns a double value. 
		On error, an exception is raised, and the value returned by 
		this function should be discarded. */

	DWORD dwType = REG_BINARY;
	double d;
	DWORD dwSize = sizeof(d);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return fDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&d, &dwSize) != ERROR_SUCCESS) d = fDefault;
	::RegCloseKey(hKey);	
	return d;
}

void GuruLib::Registry::ReadString(wchar_t* strName, wchar_t* strDefault, std::wstring& buffer)
{
	DWORD dwType = REG_SZ;
	BOOL bSuccess = TRUE;
	_TCHAR sz[255];
	DWORD dwSize = ArraySize(sz) * sizeof(wchar_t);
	HKEY hKey;
			 
	assert(m_strCurrentPath.length() > 0);

	// make sure it is the proper type
	dwType = GetDataType(strName);
	
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
	{
		buffer = strDefault;
		return;
	}

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY  | KEY_READ : KEY_READ, &hKey);
	if (m_nLastError != ERROR_SUCCESS)
	{
		buffer = strDefault;
		return;
	}
		
	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)sz, &dwSize);
	if (m_nLastError != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	
	if (!bSuccess)
	{
		buffer = strDefault;
		return;
	}
	buffer = sz;
}

DWORD GuruLib::Registry::ReadDword(wchar_t* strName, DWORD dwDefault)
{
	DWORD dwType = REG_DWORD;
	DWORD dw;
	DWORD dwSize = sizeof(dw);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return dwDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&dw, &dwSize) != ERROR_SUCCESS) dw = dwDefault;
	::RegCloseKey(hKey);	
	return dw;
}

void GuruLib::Registry::ReadByteArray(wchar_t* strName, BYTE* buffer, DWORD length)
{
	DWORD dwType = REG_BINARY;
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process? KEY_WOW64_64KEY |KEY_READ :KEY_READ, &hKey) != ERROR_SUCCESS) return;

	::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)buffer, &length);
	::RegCloseKey(hKey);
}

int GuruLib::Registry::ReadInt(wchar_t* strName, int nDefault)
{
	DWORD dwType = REG_BINARY;
	int n;
	DWORD dwSize = sizeof(n);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return nDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&n, &dwSize) != ERROR_SUCCESS) n = nDefault;
	::RegCloseKey(hKey);	
	return n;
}

BOOL GuruLib::Registry::ReadBool(wchar_t* strName, BOOL bDefault)
{
	DWORD dwType = REG_BINARY;
	BOOL b;
	DWORD dwSize = sizeof(b);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return bDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&b, &dwSize) != ERROR_SUCCESS) b = bDefault;
	::RegCloseKey(hKey);	
	return b;
}

void GuruLib::Registry::ReadColor(wchar_t* strName, COLORREF rgbDefault, COLORREF& rgb_buffer)
{
	DWORD dwType = REG_BINARY;
	COLORREF rgb;
	DWORD dwSize = sizeof(rgb);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		rgb_buffer = rgbDefault;
		return;
	}

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&rgb, &dwSize) != ERROR_SUCCESS)
	{
		rgb_buffer = rgbDefault;
		return;
	}
	::RegCloseKey(hKey);	
	rgb_buffer = rgb;
}

//BOOL GuruLib::Registry::ReadFont(wchar_t* strName, CFont* pFont)
//{
//	DWORD dwType = REG_BINARY;
//	DWORD dwSize = sizeof(LOGFONT);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//	LOGFONT lf;
//
//	assert(m_strCurrentPath.length() > 0);
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;
//
//	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
//		&dwType, (LPBYTE)&lf, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
//	::RegCloseKey(hKey);	
//	if (bSuccess)
//	{
//		pFont->Detach();
//		pFont->CreateFontIndirect(&lf);
//	}
//	return bSuccess;
//}

//BOOL GuruLib::Registry::ReadPoint(wchar_t* strName, CPoint* pPoint)
//{
//	DWORD dwType = REG_BINARY;
//	DWORD dwSize = sizeof(CPoint);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	assert(m_strCurrentPath.length() > 0);
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY | KEY_READ : KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;
//
//	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
//		&dwType, (LPBYTE)pPoint, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
//	::RegCloseKey(hKey);	
//	return bSuccess;
//}

//BOOL GuruLib::Registry::ReadSize(wchar_t* strName, CSize* pSize)
//{
//	DWORD dwType = REG_BINARY;
//	DWORD dwSize = sizeof(CSize);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	assert(m_strCurrentPath.length() > 0);
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_READ:KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;
//
//	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
//		&dwType, (LPBYTE)pSize, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
//	::RegCloseKey(hKey);	
//	return bSuccess;
//}

//BOOL GuruLib::Registry::ReadRect(wchar_t* strName, CRect* pRect)
//{
//	DWORD dwType = REG_BINARY;
//	DWORD dwSize = sizeof(CRect);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	assert(m_strCurrentPath.length() > 0);
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_READ :KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;
//
//	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
//		&dwType, (LPBYTE)pRect, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
//	::RegCloseKey(hKey);	
//	return bSuccess;
//}

BOOL GuruLib::Registry::WriteBool(wchar_t* strName, BOOL bValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&bValue, sizeof(bValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

//BOOL GuruLib::Registry::WriteDateTime(wchar_t* strName, COleDateTime dtValue)
//{
//	assert(m_strCurrentPath.length() > 0);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
//	
//	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
//		REG_BINARY, (LPBYTE)&dtValue, sizeof(dtValue))
//		 != ERROR_SUCCESS) bSuccess = FALSE;
//		
//	if (!m_bLazyWrite) ::RegFlushKey(hKey);
//	::RegCloseKey(hKey);
//	return bSuccess;
//}

BOOL GuruLib::Registry::WriteString(wchar_t* strName, const wchar_t* value)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	
	if (wcslen(value) > 254) return FALSE;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
#ifdef _UNICODE
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_SZ, (LPBYTE)value, (wcslen(value) + 1)*sizeof(wchar_t))
		 != ERROR_SUCCESS) bSuccess = FALSE;
#else
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_SZ, (LPBYTE)strValue, (strlen(strValue) + 1)*sizeof(char))
		 != ERROR_SUCCESS) bSuccess = FALSE;
#endif
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL GuruLib::Registry::WriteFloat(wchar_t* strName, double fValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&fValue, sizeof(fValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL GuruLib::Registry::WriteInt(wchar_t* strName, int nValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&nValue, sizeof(nValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL GuruLib::Registry::WriteDword(wchar_t* strName, DWORD dwValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&dwValue, sizeof(dwValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL GuruLib::Registry::WriteColor(wchar_t* strName, COLORREF rgbValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE : KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&rgbValue, sizeof(rgbValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL GuruLib::Registry::WriteByteArray(wchar_t* strName, BYTE* buffer, DWORD length)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		is_wow64_process ? KEY_WOW64_64KEY | KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)buffer, length)
		!= ERROR_SUCCESS) bSuccess = FALSE;

	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

//BOOL GuruLib::Registry::WriteFont(wchar_t* strName, CFont* pFont)
//{
//	assert(m_strCurrentPath.length() > 0);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	LOGFONT lf;
//	pFont->GetLogFont(&lf);
//
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
//	
//	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
//		REG_BINARY, (LPBYTE)&lf, sizeof(lf))
//		 != ERROR_SUCCESS) bSuccess = FALSE;
//		
//	if (!m_bLazyWrite) ::RegFlushKey(hKey);
//	::RegCloseKey(hKey);
//	return bSuccess;
//}

//BOOL GuruLib::Registry::WritePoint(wchar_t* strName, CPoint* pPoint)
//{
//	ASSERT(m_strCurrentPath.length() > 0);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
//	
//	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
//		REG_BINARY, (LPBYTE)pPoint, sizeof(CPoint))
//		 != ERROR_SUCCESS) bSuccess = FALSE;
//		
//	if (!m_bLazyWrite) ::RegFlushKey(hKey);
//	::RegCloseKey(hKey);
//	return bSuccess;
//}

//BOOL GuruLib::Registry::WriteSize(wchar_t* strName, CSize* pSize)
//{
//	ASSERT(m_strCurrentPath.length() > 0);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
//	
//	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
//		REG_BINARY, (LPBYTE)pSize, sizeof(CSize))
//		 != ERROR_SUCCESS) bSuccess = FALSE;
//		
//	if (!m_bLazyWrite) ::RegFlushKey(hKey);
//	::RegCloseKey(hKey);
//	return bSuccess;
//}

//BOOL GuruLib::Registry::WriteRect(wchar_t* strName, CRect* pRect)
//{
//	ASSERT(m_strCurrentPath.length() > 0);
//	BOOL bSuccess = TRUE;
//	HKEY hKey;
//
//	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
//		is_wow64_process? KEY_WOW64_64KEY |KEY_WRITE:KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
//	
//	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
//		REG_BINARY, (LPBYTE)pRect, sizeof(CRect))
//		 != ERROR_SUCCESS) bSuccess = FALSE;
//		
//	if (!m_bLazyWrite) ::RegFlushKey(hKey);
//	::RegCloseKey(hKey);
//	return bSuccess;
//}

