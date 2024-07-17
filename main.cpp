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
        // steer -> https://github.com/EdoardoTorrini/CanOpenWrapper/blob/9dc5f1910123b0622bce2c41ec691d22bb819d4e/main.cpp#L20
        // Terzo argomento: sara' il message ID dei messaggi mandati con download
        // 600 di default così è simile alla macchina 
        pCanOpen = new CANOpen(0x600, wrapper, 11, 0x580);      


        // pCanOpen->download<uint8_t>(0x6060, 0x00, static_cast<uint8_t>(0x01));
        // self.download(0x6040, 0x00, b"\x06\x00")
        // self.download(0x6040, 0x00, b"\x0F\x00")

        // Enable the device
        pCanOpen->download<uint16_t>(0x6040, 0x01, static_cast<uint16_t>(0x0605040));
        pCanOpen->download<uint16_t>(0x6040, 0x00, static_cast<uint16_t>(0x0F00));

        //upload 
        while (1) {
        
            // cout << "Faccio upload " << endl;
            bool somethingWasRead = false;
            int data = pCanOpen->upload<int>(0x6060, 0x00, somethingWasRead);
            // cout << "Data: " << data << endl;
            if (somethingWasRead) 
                printf("------\nupload ha ritornato: %x \n------\n", data);
            
            
            
        }

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