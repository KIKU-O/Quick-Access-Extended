#include "Handler.h"

Handler::Handler()
{
	_InterlockedIncrement(&REFERENCE);
}

HRESULT Handler::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax)
{
	return E_NOTIMPL;
}

HRESULT Handler::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
	std::string lpPath = pici->lpDirectory + std::string("\\");
	std::string dllPath = DLL_QAEPATH_S.c_str() + std::string("\\");
	if(std::filesystem::is_directory(lpPath) && std::filesystem::is_directory(dllPath))
	{
		try
		{
			std::filesystem::create_directory_symlink(lpPath.c_str(), dllPath.c_str());
			return S_OK;
		}
		catch (std::filesystem::filesystem_error& e)
		{
			MessageBoxA (NULL, "FileSystem Error!", e.what(), MB_OK);
			return E_ABORT;
		}
	}
}

HRESULT Handler::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	if
	(uFlags & CMF_DEFAULTONLY)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

	MENUITEMINFO ITEM = {};
	ITEM.cbSize = sizeof(MENUITEMINFO);
	ITEM.fMask = MIIM_STRING | MIIM_ID;
	ITEM.wID = idCmdFirst + 1;
	ITEM.dwTypeData = const_cast<LPTSTR>(L"Pin to Quick Access Extended.");

	if(!InsertMenuItemW(hmenu, 0, TRUE, &ITEM))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, (ITEM.wID - idCmdFirst) + 1);
}

ULONG __stdcall Handler::AddRef()
{
	return _InterlockedIncrement(&REFERENCE);
}

ULONG __stdcall Handler::Release()
{
	unsigned int value = _InterlockedDecrement(&REFERENCE);
	if (value < 1)
	{
		delete this;
	}
	return value;
}

HRESULT __stdcall Handler::QueryInterface(REFIID riid, void** ppv)
{
	if (!ppv)
	{
		return E_POINTER;
	}
	*ppv = nullptr;

	if (IsEqualIID(riid, IID_IUnknown))
	{
		*ppv = this;
		this->AddRef();
		return S_OK;
	}
	else if (IsEqualIID(riid, IID_IContextMenu))
	{
		*ppv = (IContextMenu*)this;
		this->AddRef();
		return S_OK;
	}
	else if (IsEqualIID(riid, IID_IShellExtInit))
	{
		*ppv = (IShellExtInit*)this;
		this->AddRef();
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}

	return E_NOTIMPL;
}

HRESULT Handler::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject* pdtobj, HKEY hKeyProgID)
{
	return S_OK;
}

Handler::~Handler()
{
	_InterlockedDecrement(&REFERENCE);
}