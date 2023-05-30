#pragma once

#include <atlbase.h>
#include <atomic>
#include <memory>
#include <string>
#include <windows.h>

namespace ComEventHandler
{
    template <class ComClient, typename ComInterface, typename ComEventInterface>
    class ComEventHandler : IDispatch
    {
        // type definition for IDispatch::Invoke, same usage as in com server.
        // See Invoke usage under _ICollisionClientStatusEvents_CP.h
        // here we are creating type for Sink (Connection) to that Invoked method in com server
        using onEventFiringObjectInvoke = HRESULT(ComClient::*)
            (
                ComEventHandler<ComClient, ComInterface, ComEventInterface>* pthis,
                DISPID dispidMember,
                REFIID riid,
                LCID lcid,
                WORD wFlags,
                DISPPARAMS* pdispparams,
                VARIANT* pvarResult,
                EXCEPINFO* pexcepinfo,
                UINT* puArgErr
                );

    public:
        ComEventHandler(ComClient& comClient, ComInterface* comInterface, onEventFiringObjectInvoke comClientOnInvoke)
            : mComClient(comClient)
            , mComClientOnInvoke(comClientOnInvoke)
        {
            setupConnectionPoint(comInterface);
        }

        ~ComEventHandler()
        {
            shutdownConnectionPoint();
        }

        STDMETHOD_(ULONG, AddRef)()
        {
            return ++mRefCount;
        }

        STDMETHOD_(ULONG, Release)()
        {
            --mRefCount;
            if (mRefCount == 0)
            {
                delete this;
                return 0;
            }
            return mRefCount;
        }

        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
        {
            if (riid == IID_IUnknown)
            {
                *ppvObject = (IUnknown*)this;
                AddRef();
                return S_OK;
            }

            if ((riid == IID_IDispatch) || (riid == __uuidof(ComEventInterface)))
            {
                *ppvObject = (IDispatch*)this;
                AddRef();
                return S_OK;
            }

            return E_NOINTERFACE;
        }

        STDMETHOD(GetTypeInfoCount)(UINT* /*pctinfo*/)
        {
            return E_NOTIMPL;
        }

        STDMETHOD(GetTypeInfo)(UINT /*itinfo*/, LCID /*lcid*/, ITypeInfo** /*pptinfo*/)
        {
            return E_NOTIMPL;
        }

        STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* /*rgszNames*/, UINT /*cNames*/,
            LCID /*lcid*/, DISPID* /*rgdispid*/)
        {
            return E_NOTIMPL;
        }

        STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
            LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
            EXCEPINFO* pexcepinfo, UINT* puArgErr)
        {
            // here we trigger OnInvoke method of com client
            return (mComClient.*mComClientOnInvoke)(this, dispidMember, riid, lcid, wFlags, pdispparams, pvarResult, pexcepinfo, puArgErr);
        }

    protected:
        std::atomic<int> mRefCount;
        ComClient& mComClient; // owner of this handler
        IConnectionPoint* mIConnectionPoint = nullptr;
        DWORD mAdviseCookie{ 0 };
        onEventFiringObjectInvoke mComClientOnInvoke;

        void setupConnectionPoint(ComInterface* comInterface)
        {
            IConnectionPointContainer* pIConnectionPointContainerTemp = NULL;
            // get connection point from com server
            comInterface->QueryInterface(IID_IConnectionPointContainer, (void**)&pIConnectionPointContainerTemp);

            if (pIConnectionPointContainerTemp)
            {
                pIConnectionPointContainerTemp->FindConnectionPoint(__uuidof(ComEventInterface), &mIConnectionPoint);
                pIConnectionPointContainerTemp->Release();
                pIConnectionPointContainerTemp = NULL;
            }

            if (mIConnectionPoint)
            {
                // Establish a connection between a connection point object and the client's sink.
                // see https://learn.microsoft.com/en-us/windows/win32/api/ocidl/nf-ocidl-iconnectionpoint-advise
                mIConnectionPoint->Advise((IUnknown*)this, &mAdviseCookie); // this = ComEventHandler -> IDispatch -> IUnknown
            }
        }

    private:
        void shutdownConnectionPoint()
        {
            if (mIConnectionPoint)
            {
                mIConnectionPoint->Unadvise(mAdviseCookie);
                mAdviseCookie = 0;
                mIConnectionPoint->Release();
                mIConnectionPoint = nullptr;
            }
        }
    };

} // end-of-namespace ComEventHandler
