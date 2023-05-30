#pragma once

#include "ComEventHandler.h"

#include <unordered_map>

#import "../ATLCOM/x64/Debug/ATLCOM.tlb"
using namespace ATLCOMLib;

namespace ComClient
{
    class ComClient
    {
        using IComEventHandler = ComEventHandler::ComEventHandler<ComClient, IMyEventObject, _IMyEventObjectEvents>;

    public:
        ComClient();

    protected:
        IMyEventObjectPtr mComInterface; // smart pointer to com interface
        IComEventHandler* mIComEventHandler;

        HRESULT onEventFiringObjectInvoke
        (
            IComEventHandler* pEventHandler,
            DISPID dispidMember,
            REFIID riid,
            LCID lcid,
            WORD wFlags,
            DISPPARAMS* pdispparams,
            VARIANT* pvarResult,
            EXCEPINFO* pexcepinfo,
            UINT* puArgErr
        );

    private:
        void handleFireEventTest(DISPPARAMS* pdispparams);

        enum class EventsFiredFromCOMServer
        {
            FireEventTest,
        };

        static inline const std::unordered_map<int, EventsFiredFromCOMServer> mCOMEventsMap
        {
            {1, EventsFiredFromCOMServer::FireEventTest},
        };

    };
} // end-of-namespace ComClient
