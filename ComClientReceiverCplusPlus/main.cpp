#include "Utility.h"

int main()
{
    std::thread comReceiverGetMessageThread(Utility::Utility::startReceivingViaGetMessage);
    std::thread additionalWorkThanCOM(Utility::Utility::doSomeAdditionalOperations);
    // we can also use PeekMessage but currently we prefered GetMessage on collision client
    //std::thread comReceiverPeekMessageThread(Utility::Utility::startReceivingViaPeekMessage);

    comReceiverGetMessageThread.join();
    additionalWorkThanCOM.join();
    //comReceiverPeekMessageThread.join();

    std::cin.get();
    return 0;
}