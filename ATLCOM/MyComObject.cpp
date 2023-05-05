// MyComObject.cpp : Implementation of CMyComObject

#include "pch.h"
#include "MyComObject.h"


// CMyComObject

#include <exception>

STDMETHODIMP CMyComObject::MakeSomeCalculation(DOUBLE* input)
{
    HRESULT result = S_OK;
    try
    {
        if (input)
        {
            *input *= *input;
        }

    }
    catch (const std::exception&)
    {
        result = S_FALSE;
    }

    return result;
}


STDMETHODIMP CMyComObject::Speak(CHAR* param)
{
    param = "Greetings from COM Server!\0";

    return S_OK;
}
