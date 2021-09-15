#pragma once
#include "C-Linker.h"

extern unsigned int
COJBC;

class Selector : IClassFactory
{
protected:
	DWORD REFERENCE = NULL;
public:
	Selector();

	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);

	HRESULT __stdcall CreateInstance(IUnknown* LSR, REFIID riid, void** ppv);
	HRESULT __stdcall LockServer(BOOL value);
protected:
	~Selector();
};