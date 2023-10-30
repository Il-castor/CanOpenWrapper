#include "canbus_network.hpp"
#include "canopen.hpp"
#include "canbus.hpp"

using namespace CanNetworkBase;
using namespace CanOpenWrapper;
using namespace CanBusBase;

int main(int argc, char* argv[])
{
    CanNetwork* pSocketCan = NULL;
    CANBus* pBus = NULL;
    CANOpen* pCanOpen = NULL;
    CANOpen* pCanOpen_ = NULL;

    try 
    {
        pSocketCan = new CanNetwork("can0", 1000000);
        
        // steer
        pCanOpen = new CANOpen(18, pSocketCan->getSocket(), 0x600, 0x580);

        pCanOpen->download<uint8_t>(0x6060, 0x00, static_cast<uint8_t>(0x01));
        pCanOpen->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0600));
        pCanOpen->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0F00));
        
        // brake
        pCanOpen_ = new CANOpen(1, pSocketCan->getSocket(), 0x600, 0x580);

        pCanOpen_->download<uint8_t>(0x6060, 0x00, static_cast<uint8_t>(0x01));
        pCanOpen_->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0600));
        pCanOpen_->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0F00));

    }
    catch (CANException &canError)
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
    
    if (pCanOpen != NULL)
        delete pCanOpen;

    if (pCanOpen_ != NULL)
        delete pCanOpen_;

    return 0;
}