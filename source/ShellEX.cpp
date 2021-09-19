#include "ShellEX.h"

HINSTANCE g_hInstance {};

unsigned int COJBC = NULL;

BOOL __stdcall DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case (DLL_PROCESS_ATTACH):
		g_hInstance = hInstDLL;
		return true;
		break;
	default:
		break;
	}

	return true;
}

#pragma warning (disable: 28251)
HRESULT __stdcall DllCanUnloadNow(void)
{
	return { COJBC > 0 ? S_FALSE : S_OK };
}
#pragma warning (default: 28251)

#pragma warning (disable: 28251)
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
	if (!IsEqualCLSID(rclsid, EX_CLSID))
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	if (!ppv)
	{
		return E_INVALIDARG;
	}
	*ppv = NULL;

	HRESULT hr = E_UNEXPECTED;
	Selector* SLC = new Selector();
	if (SLC != nullptr)
	{
		hr = SLC->QueryInterface(riid, ppv);
		SLC->Release();
	}

	return
	hr;
}
#pragma warning (default: 28251)

HRESULT __stdcall DllRegisterServer(void)
{
	HKEY hKey {};
	DWORD lpDisp {};
	std::wstring lpSubKey {};

#pragma region Global
	#pragma region GUID Key
		// GUID Key
		lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID();
		unsigned int RESULT = RegCreateKeyExW
		(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}


		RESULT = RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)DLL_SUBNAME.c_str(), SizeInBytes(DLL_SUBNAME));
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	#pragma endregion

	#pragma region System Pin Key
		// System Pin Key
		RESULT = RegSetValueExW(hKey, L"System.IsPinnedToNameSpaceTree", 0, REG_DWORD, (const BYTE*)&DLL_SYSTEM_PIN, sizeof(DLL_SYSTEM_PIN));
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	#pragma endregion

	#pragma region System Order Key
		// System Order Key
		RESULT = RegSetValueExW(hKey, L"SortOrderIndex", 0, REG_DWORD, (const BYTE*)&DLL_SYSTEM_ORDER, sizeof(DLL_SYSTEM_ORDER));
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	#pragma endregion

	#pragma region CLSID//...
		#pragma region ...//InProcServer32
			// InProcServer32 Key
			RESULT = RegCreateKeyExW(hKey, L"InProcServer32", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Default Variables
			wchar_t buffer[MAX_PATH];
			GetModuleFileNameW(g_hInstance, buffer, MAX_PATH);
			std::wstring lpDllPath = std::wstring(buffer);
			RESULT = RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)lpDllPath.c_str(), SizeInBytes(lpDllPath));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Threading Model
			std::wstring apartment = L"Apartment";
			RESULT = RegSetValueExW
			(hKey, L"ThreadingModel", 0, REG_SZ, (BYTE*)apartment.c_str(), SizeInBytes(apartment));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}
			RegCloseKey(hKey);
		#pragma endregion

		#pragma region .../ShellFolder
			// Shell Folder Key (Higher)
			RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			RESULT = RegCreateKeyExW(hKey, L"ShellFolder", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Folder Value Flags Key
			RESULT = RegSetValueExW(hKey, L"FolderValueFlags", 0, REG_DWORD, (const BYTE*)&DLL_FOLDER_FLAGS, sizeof(DLL_FOLDER_FLAGS));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Folder Attributes Key
			RESULT = RegSetValueExW(hKey, L"Attributes", 0, REG_DWORD, (const BYTE*)&DLL_SHELL_ATTRIBUTES, sizeof(DLL_SHELL_ATTRIBUTES));
			if(RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}
			RegCloseKey(hKey);
		#pragma endregion

		#pragma region .../Instance
			// Instance Folder Key (Higher)
			RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			RESULT = RegCreateKeyExW(hKey, L"Instance", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if(RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Instance CLSID Link
			std::wstring EXTERNAL_GUID = L"{0E5AAE11-A475-4c5b-AB00-C66DE400274E}";
			RESULT = RegSetValueExW(hKey, L"CLSID", 0, REG_SZ, (BYTE*)EXTERNAL_GUID.c_str(), SizeInBytes(EXTERNAL_GUID));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

		#pragma region ...//Instance//InitPropertyBag
			// InitPropertyBag Subkey (SubHigher)
			RESULT = RegCreateKeyExW(hKey, L"InitPropertyBag", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Folder Attribution
			RESULT = RegSetValueExW(hKey, L"Attributes", 0, REG_DWORD, (const BYTE*)&DLL_FOLDER_ATTRIBUTE, sizeof(DLL_FOLDER_ATTRIBUTE));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			// Folder Path
			const char* TPATH = DLL_QAEPATH_S.c_str();
			if (GetFileAttributesA(TPATH) != FILE_ATTRIBUTE_DIRECTORY)
			{
				RESULT = _mkdir(DLL_QAEPATH_S.c_str());
				if (RESULT != ERROR_SUCCESS)
				{
					return E_UNEXPECTED;
				}
			}

			RESULT = RegSetValueExW(hKey, L"TargetFolderPath", 0, REG_SZ, (BYTE*)DLL_QAEPATH.c_str(), SizeInBytes(DLL_QAEPATH));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}
			RegCloseKey(hKey);
		#pragma endregion
		#pragma endregion

		#pragma region .../DefaultIcon
			// Icon Key (Higher)
			RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			RESULT = RegCreateKeyExW(hKey, L"DefaultIcon", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}

			RESULT = RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)DLL_ICONPATH.c_str(), SizeInBytes(DLL_ICONPATH));
			if (RESULT != ERROR_SUCCESS)
			{
				return E_UNEXPECTED;
			}
			RegCloseKey(hKey);
		#pragma endregion
	#pragma endregion

	#pragma region Handler Key
		// Handler Key
		lpSubKey = DLL_FORMAT + DLL_NAME;
		RESULT = RegCreateKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
		RESULT = RegSetValueExW(hKey, NULL, 0, REG_SZ, ((BYTE*)GetCLSID().c_str()), SizeInBytes(GetCLSID()));
		if(RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
		RegCloseKey(hKey);
	#pragma endregion

	#pragma region Microsoft
		#pragma region Approved List
			// Approved List
			lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved";
			RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			if (RESULT == ERROR_SUCCESS)
			{ 
				RegSetValueExW(hKey, GetCLSID().c_str(), 0, REG_SZ, (BYTE*)DLL_NAME.c_str(), SizeInBytes(DLL_NAME));
				if (RESULT != ERROR_SUCCESS)
				{
					return E_UNEXPECTED;
				}
			}
			RegCloseKey(hKey);
		#pragma endregion

		#pragma region Desktop Namespace
			// Desktop Namespace
			lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\" + GetCLSID();
			RESULT = RegCreateKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &lpDisp);
			if (RESULT == ERROR_SUCCESS)
			{
				RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)DLL_SUBNAME.c_str(), SizeInBytes(DLL_SUBNAME));
				if (RESULT != ERROR_SUCCESS)
				{
					return E_UNEXPECTED;
				}
			}
			RegCloseKey(hKey);
		#pragma endregion

		#pragma region Panel Key
			// Panel Key
			lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel";
			RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			if (RESULT == ERROR_SUCCESS)
			{
				RESULT = RegSetValueExW(hKey, GetCLSID().c_str(), 0, REG_DWORD, (const BYTE*)&DLL_SYSTEM_GUID, sizeof(DLL_SYSTEM_GUID));
				if (RESULT != ERROR_SUCCESS)
				{
					return E_UNEXPECTED;
				}
			}
			RegCloseKey(hKey);
		#pragma endregion
	#pragma endregion
#pragma endregion

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	return S_OK;
}

HRESULT __stdcall
DllUnregisterServer
(void)
{
	HKEY hKey {};
	std::wstring lpSubKey {};

#pragma region InProcServer32 Key Deletion
	// InProcServer32 Key Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID() + L"\\InProcServer32";
	unsigned int RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS) 
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	
	}
#pragma endregion

#pragma region Shell Folder Key Deletion
	// Shell Folder Key Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID() + L"\\ShellFolder";
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region InitPropertyBag Folder Key Deletion
	// InitPropertyBag Folder Key Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID() + L"\\Instance\\InitPropertyBag";
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region Instance Folder Key Deletion
	// Instance Folder Key Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID() + L"\\Instance";
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region Icon Key Deletion
	// Icon Key Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID() + L"\\DefaultIcon";
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region GUID Deletion
	// GUID Deletion
	lpSubKey = L"SOFTWARE\\Classes\\CLSID\\" + GetCLSID();
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKey(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region Handle Deletion
	// Handle Deletion
	lpSubKey = DLL_FORMAT + DLL_NAME;
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyExW (HKEY_LOCAL_MACHINE, lpSubKey.c_str(), KEY_ALL_ACCESS, 0);
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region Approved List Deletion
	// Approved List Deletion
	lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved";
	RESULT = RegOpenKeyExW
	(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteValueW(hKey, GetCLSID().c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

#pragma region MyRegion
	// Desktop NameSpace Deletion
	lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\" + GetCLSID();
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteKeyW(HKEY_LOCAL_MACHINE, lpSubKey.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
#pragma endregion

	// Folder Path
	const char* TPATH = DLL_QAEPATH_S.c_str();
	if (GetFileAttributesA(TPATH) == FILE_ATTRIBUTE_DIRECTORY)
	{
		RESULT = _rmdir(DLL_QAEPATH_S.c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}

#pragma region Panel Key Deletion
	// Panel Key Deletion
	lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel";
	RESULT = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpSubKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if (RESULT == ERROR_SUCCESS)
	{
		RESULT = RegDeleteValueW(hKey, GetCLSID().c_str());
		if (RESULT != ERROR_SUCCESS)
		{
			return E_UNEXPECTED;
		}
	}
	RegCloseKey(hKey);
#pragma endregion

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

	return S_OK;
}