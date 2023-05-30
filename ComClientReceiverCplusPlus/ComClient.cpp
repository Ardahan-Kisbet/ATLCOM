#include "ComClient.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace ComClient
{
    ComClient::ComClient()
    {
        const auto initializeResult{ CoInitialize(nullptr) };
        auto hr = mComInterface.CreateInstance("CollisionClientStatus");
        mIComEventHandler = new IComEventHandler(*this, mComInterface, &ComClient::onEventFiringObjectInvoke);
    }

    HRESULT ComClient::onEventFiringObjectInvoke(IComEventHandler* pEventHandler, DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr)
    {
        auto firedEventID = static_cast<int>(dispidMember);
        if (!mCOMEventsMap.contains(firedEventID))
        {
            // we are not interested in all fired events
            return S_OK;
        }

        switch (mCOMEventsMap.at(firedEventID))
        {
        case EventsFiredFromCOMServer::FireEventTest:
            handleFireEventTest(pdispparams);
            break;
        default:
            break;
        }

        return S_OK;
    }

    void ComClient::handleFireEventTest(DISPPARAMS* pdispparams)
    {
        double receivedVal;

        VARIANT varValue;
        VariantInit(&varValue);
        VariantClear(&varValue);
        varValue = (pdispparams->rgvarg)[0];

        receivedVal = V_R8(&varValue);

        std::cout << std::endl << "received value: " << receivedVal << std::endl;
    }
} // end-of-namespace ComClient
