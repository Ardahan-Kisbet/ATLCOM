#pragma once

#include "ComClient.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace Utility
{
    static constexpr int WM_SUBSCRIBE_RECEIVER{ WM_APP + 0 };

    static bool handleWindowsEventMessage(const MSG& message)
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
            std::cout << "message:" << std::endl;

            std::cout << message.lParam << std::endl;
            return true;
        }

        default:
        {
            return false;
        }
        }
    }

    class Utility
    {
    public:
        // GetMessage will wait until it sees message on queue and will consume (remove) it hence no need thread-sleep
        // below we are waiting until a new message is received on message queue of calling thread
        static void startReceivingViaGetMessage()
        {
            ComClient::ComClient c;
            MSG windowsEventMessage;

            while (GetMessage(&windowsEventMessage, NULL, 0, 0) != FALSE)
            {
                if (handleWindowsEventMessage(windowsEventMessage))
                {
                    continue;
                }

                std::cout << std::endl << "com receiver using GetMessage(blocking)" << std::endl;
                DispatchMessage(&windowsEventMessage);
            }
        }

        // PeekMessage will check message queue and return immediately therefore we can sleep and try again
        // below we are checking message queue of calling thread in every seconds
        static void startReceivingViaPeekMessage()
        {
            ComClient::ComClient c;
            MSG windowsEventMessage;

            while (true)
            {
                std::cout << std::endl << "com receiver using PeekMessage(non-blocking)" << std::endl;

                while (PeekMessage(&windowsEventMessage, NULL, 0, 0, PM_REMOVE))
                {
                    if (handleWindowsEventMessage(windowsEventMessage))
                    {
                        continue;
                    }

                    DispatchMessage(&windowsEventMessage);
                }
                std::this_thread::sleep_for(1000ms);
            }
        }

        // Faking additional collision client works
        static void doSomeAdditionalOperations()
        {
            while (true)
            {
                std::cout << std::endl << "working on other operations than com.." << std::endl;
                std::this_thread::sleep_for(2000ms);
            }
        }
    };
} // end-of-namespace Utility
