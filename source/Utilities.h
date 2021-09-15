#pragma once
#include "C-Linker.h"
#include "GUID.h"

DWORD SizeInBytes(std::wstring target)
{
	return static_cast<DWORD>(((target.size() + 1) * 2));
}

std::wstring GetCLSID(void)
{
	wchar_t* TSD;
	static_cast<void>(StringFromCLSID(EX_CLSID, &TSD));
	return TSD;
}