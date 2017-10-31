#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class Registry
	{
		// Registry properties	
	private:
		HKEY m_hRootKey;
		BOOL m_bLazyWrite;
		std::wstring m_strCurrentPath;
		int m_nLastError;
		BOOL is_wow64_process = FALSE;
	protected:
	public:
		LIB inline BOOL PathIsValid() {
			return (m_strCurrentPath.length() > 0);
		}
		LIB inline const wchar_t* GetCurrentPath() {
			return m_strCurrentPath.c_str();
		}
		LIB inline HKEY GetRootKey() {
			return m_hRootKey;
		}
		//Registry	methods
		LIB explicit Registry(HKEY hRootKey);
		LIB ~Registry();
		LIB void UseWow64KeyPath(bool flag);
		LIB BOOL ClearKey();
		LIB BOOL SetRootKey(HKEY hRootKey);
		LIB BOOL CreateKey(wchar_t* strKey);
		LIB BOOL DeleteKey(wchar_t* strKey);
		LIB BOOL DeleteKey();
		LIB BOOL DeleteValue(wchar_t* strName);
		LIB int GetDataSize(wchar_t* strValueName);
		LIB DWORD GetDataType(wchar_t* strValueName);
		LIB int GetSubKeyCount();
		LIB int GetValueCount();
		LIB BOOL KeyExists(wchar_t* strKey, HKEY hRootKey = NULL);
		LIB BOOL KeyExists();
		LIB BOOL SetKey(const wchar_t* key, bool can_create = false);
		LIB BOOL ValueExists(wchar_t* strName);
		LIB void RenameValue(wchar_t* strOldName, wchar_t* strNewName);

		//reading functions
		/*COleDateTime ReadDateTime(wchar_t* strName, COleDateTime dtDefault);*/
		LIB double ReadFloat(wchar_t* strName, double fDefault);
		LIB void ReadString(wchar_t* strName, wchar_t* strDefault, std::wstring &buffer);
		LIB int ReadInt(wchar_t* strName, int nDefault);
		LIB BOOL ReadBool(wchar_t* strName, BOOL bDefault);
		LIB void ReadColor(wchar_t* strName, COLORREF rgbDefault, COLORREF& rgb_buffer);
		//TODO:should implement the commented functions in this class
		//BOOL ReadFont(wchar_t* strName, CFont* pFont);
		/*BOOL ReadPoint(wchar_t* strName, CPoint* pPoint);*/
		/*BOOL ReadSize(wchar_t* strName, CSize* pSize);*/
		/*BOOL ReadRect(wchar_t* strName, CRect* pRect);*/
		LIB DWORD ReadDword(wchar_t* strName, DWORD dwDefault);
		LIB void ReadByteArray(wchar_t* strName, BYTE* buffer, DWORD length);

		//writing functions
		LIB BOOL WriteBool(wchar_t* strName, BOOL bValue);
		/*BOOL WriteDateTime(wchar_t* strName, COleDateTime dtValue);*/
		LIB BOOL WriteString(wchar_t* strName, const wchar_t* value);
		LIB BOOL WriteFloat(wchar_t* strName, double fValue);
		LIB BOOL WriteInt(wchar_t* strName, int nValue);
		LIB BOOL WriteColor(wchar_t* strName, COLORREF rgbValue);
		LIB BOOL WriteByteArray(wchar_t* strName, BYTE* buffer, DWORD length);
		/*BOOL WriteFont(wchar_t* strName, CFont* pFont);
		BOOL WritePoint(wchar_t* strName, CPoint* pPoint);
		BOOL WriteSize(wchar_t* strName, CSize* pSize);
		BOOL WriteRect(wchar_t* strName, CRect* pRect);*/
		LIB BOOL WriteDword(wchar_t* strName, DWORD dwValue);
	};
}



