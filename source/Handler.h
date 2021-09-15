#pragma once
#include "C-Linker.h"
#include "Defenitions.h"

#include <filesystem>

extern unsigned int
COJBC;

class Handler : public IShellExtInit, IContextMenu
{
protected:
	DWORD REFERENCE;
public:
	Handler();

	HRESULT GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax);
	HRESULT InvokeCommand(LPCMINVOKECOMMANDINFO pici);
	HRESULT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);

	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);

	HRESULT Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject* pdtobj, HKEY hKeyProgID);
protected:
	~Handler();
};