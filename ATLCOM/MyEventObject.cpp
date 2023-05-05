// MyEventObject.cpp : Implementation of CMyEventObject

#include "pch.h"
#include "MyEventObject.h"


// CMyEventObject



STDMETHODIMP CMyEventObject::SimpleEvent()
{
    DOUBLE valueToBeSent = 0.1;
    Fire_FireEventTest(valueToBeSent);

    return S_OK;
}
