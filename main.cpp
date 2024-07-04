#include "canbus_network.hpp"
#include "canopen.hpp"
#include "canbus.hpp"
#include <iostream>

using namespace CanNetworkBase;
using namespace CanOpenWrapper;
using namespace CanBusBase;
using namespace std;

int main(int argc, char* argv[])
{
    CanNetwork* pSocketCan = NULL;
    CANBus* pBus = NULL;
    CANOpen* pCanOpen = NULL;
    CANOpen* pCanOpen_ = NULL;
    CanBusWrapper* wrapper = NULL;

    try 
    {
        pSocketCan = new CanNetwork("vcan0", 1000000);
        
        wrapper = new CanBusWrapper(pSocketCan->getSocket(),  0, 0xFF);
        // // steer
        // Terzo argomento: sara' il message ID dei messaggi mandati con download
        // 600 di default così è simile alla macchina 
        pCanOpen = new CANOpen(0x600, wrapper, 11, 0x580);      


        //pCanOpen->download<uint8_t>(0x6060, 0x00, static_cast<uint8_t>(0x01));
        //pCanOpen->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0600));
        // pCanOpen->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0F00));

        //upload 
        
        cout << "Faccio upload " << endl;
        int data = pCanOpen->upload<int>(0x6060, 0x00);
        cout << "Data: " << data << endl;


        // // brake
        // pCanOpen_ = new CANOpen(1, wrapper, 0x600, 0x580);

        // pCanOpen_->download<uint8_t>(0x6060, 0x00, static_cast<uint8_t>(0x01));
        // pCanOpen_->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0600));
        // pCanOpen_->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0F00));

    }
    catch (CANException &canError)
    { 
        printf(
            "CANException [ ID ]: %d, [ MSG ]: %s\n", 
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