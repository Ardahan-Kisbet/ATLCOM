// MyEventObject.h : Declaration of the CMyEventObject

#pragma once
#include "resource.h"       // main symbols



#include "ATLCOM_i.h"
#include "_IMyEventObjectEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CMyEventObject

class ATL_NO_VTABLE CMyEventObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyEventObject, &CLSID_MyEventObject>,
	public IConnectionPointContainerImpl<CMyEventObject>,
	public CProxy_IMyEventObjectEvents<CMyEventObject>,
	public IDispatchImpl<IMyEventObject, &IID_IMyEventObject, &LIBID_ATLCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyEventObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYEVENTOBJECT)

// To able to talk through same com instance between multiple clients
DECLARE_CLASSFACTORY_SINGLETON(CMyEventObject)

BEGIN_COM_MAP(CMyEventObject)
	COM_INTERFACE_ENTRY(IMyEventObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMyEventObject)
	CONNECTION_POINT_ENTRY(__uuidof(_IMyEventObjectEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SimpleEvent)();
};

OBJECT_ENTRY_AUTO(__uuidof(MyEventObject), CMyEventObject)
