#include "Selector.h"
#include "Handler.h"

Selector::Selector()
{
	_InterlockedIncrement(&REFERENCE);
}

ULONG __stdcall Selector::AddRef()
{
	return _InterlockedIncrement(&REFERENCE);
}

ULONG __stdcall Selector::Release()
{
	unsigned int value = _InterlockedDecrement(&REFERENCE);
	if (value < 1)
	{
		delete this;
	}
	return value;
}

HRESULT __stdcall Selector::QueryInterface(REFIID riid, void** ppv)
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
	else if (IsEqualIID(riid, IID_IClassFactory))
	{
		*ppv = (IClassFactory*)this;
		this->AddRef();
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}

	return E_NOTIMPL;
}

HRESULT __stdcall Selector::CreateInstance (IUnknown* LSR, REFIID riid, void** ppv)
{
	if (ppv == nullptr)
	{
		return E_INVALIDARG;
	}

	if (LSR != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	HRESULT hr;
	if (IsEqualIID(riid, IID_IShellExtInit) || IsEqualIID(riid, IID_IContextMenu))
	{
		Handler* CMH = new Handler();
		if (CMH == nullptr)
		{
			return E_OUTOFMEMORY;
		}

		hr = CMH->QueryInterface(riid, ppv);
		CMH->Release();
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}

HRESULT __stdcall Selector::LockServer (BOOL fLock)
{
	return S_OK;
}

Selector::~Selector()
{
	_InterlockedDecrement (&REFERENCE);
}