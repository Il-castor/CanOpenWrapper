#include "canbus_network.hpp"
#include "canbus.hpp"

using namespace CanNetworkBase;
using namespace CanBusBase;

int main(int argc, char* argv[])
{
    CanNetwork* pSocketCan = NULL;
    CANBus* pBus = NULL;

    try 
    {
        pSocketCan = new CanNetwork("vcan0", 500000);
        int nSocket = pSocketCan->getSocket();

        printf("[ SOCKET ]: %d\n", nSocket);

        pBus = new CANBus(nSocket);

        struct can_frame frame;
        frame.can_id = 0x01;
        frame.can_dlc = 1;
        frame.data[0] = 0x02;

        pBus->writeData(frame);

    }
    catch (CanOpenException &canError)
    { 
        printf(
            "[ ID ]: %d, [ MSG ]: %s\n", 
            canError.getStatusCode(), canError.getMsg().c_str()
        ); 
    }
    catch (std::exception &eErr)
    {
        printf("[ ERROR MSG ]: %s\n", eErr.what());
    }


    sleep(30);
    delete pSocketCan;

    if (pBus != NULL)
        delete pBus;

    return 0;
}