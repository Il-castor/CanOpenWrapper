#include "canbus_network.hpp"
#include "canbus_wrapper.hpp"

using namespace CanNetworkBase;
using namespace CanBusBase;

int main(int argc, char* argv[])
{
    CanNetwork* pSocketCan = NULL;
    CanBusWrapper* pCanBus = NULL;

    try 
    {
        pSocketCan = new CanNetwork("vcan0", 500000);
        int nSocket = pSocketCan->getSocket();

        printf("[ SOCKET ]: %d\n", nSocket);

        pCanBus = new CanBusWrapper(nSocket);

        // test canbus frame send
        struct can_frame frame;
        frame.can_id = 0x01;
        frame.can_dlc = 1;
        frame.data[0] = 0x02;
        
        pCanBus->writeData(frame);
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
    delete pCanBus;

    return 0;
}