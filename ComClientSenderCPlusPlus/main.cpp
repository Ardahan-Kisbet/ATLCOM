#include <iostream>
#include <string>

#include <objbase.h>
#include <atlbase.h>
#include <atlcom.h>

#import "../ATLCOM/x64/Debug/ATLCOM.tlb"
using namespace ATLCOMLib;

static constexpr int WM_SUBSCRIBE_RECEIVER{ WM_APP + 0 };
IMyEventObjectPtr myComPtr;

bool handleWindowsEventMessage(const MSG& message)
{
    switch (message.message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return true;
    }

    case WM_SUBSCRIBE_RECEIVER:
    {
        std::cout << "message: " << std::endl;
        std::cout << message.lParam << std::endl;
        return true;
    }
    default:
    {
        return false;
    }
    }
}


void CALLBACK transmitData(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
    HRESULT result{ E_FAIL };

    try
    {
        result = myComPtr->SimpleEvent();
    }
    catch (...)
    {
        std::cout << "Exception occurred during transmit call" << std::endl;
    }

    if (SUCCEEDED(result))
    {
        auto temp = result == 0 ? "S_OK" : std::to_string(result);
        std::cout << std::endl << "sent data " << "(" << temp << ")" << std::endl;
    }
    else
    {
        std::cout << std::endl << "transmit data failed" << std::endl;
    }
}

bool initCom()
{
    bool result;
    auto initResult{ CoInitialize(nullptr) };

    if (SUCCEEDED(initResult))
    {
        result = true;
    }
    else
    {
        _com_error error{ initResult };
        std::wstring errorMessage{ error.ErrorMessage() };
        std::wcout << L"CoInitialize failed with error: " << errorMessage << std::endl;
        result = false;
    }

    return result;
}

void runTest()
{
    MSG windowsEventMessage;
    auto hr{ myComPtr.CreateInstance(__uuidof(MyEventObject), nullptr, CLSCTX_LOCAL_SERVER) };

    if (SUCCEEDED(hr))
    {
        SetTimer(NULL, 0, 1000, (TIMERPROC)&transmitData);

        while (GetMessage(&windowsEventMessage, NULL, 0, 0) != FALSE)
        {
            if (handleWindowsEventMessage(windowsEventMessage))
            {
                continue;
            }
            DispatchMessage(&windowsEventMessage);
        }
    }
    else
    {
        _com_error error{ hr };
        std::wstring errorMessage{ error.ErrorMessage() };
        std::wcout << L"Could not create COM instance: " << errorMessage << std::endl;
    }
}


int main()
{
    int exitCode{ 0 };

    if (initCom())
    {
        runTest();
    }
    else
    {
        exitCode = -1;
    }

    return exitCode;
}
