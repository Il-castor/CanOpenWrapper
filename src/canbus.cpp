#include "canbus.hpp"


CANBus::CANBus(int nSocketCan, int nCanID, int nCanMask)
: CanBusBase::CanBusWrapper(nSocketCan, nCanID, nCanMask) { }

void CANBus::canBusListener(struct can_frame cfd) 
{
    printf("[ ID ]: %d, [ DLC ]: %d, [ DATA ]: ", cfd.can_id, cfd.can_dlc);
    for (int i = 0; i < cfd.can_dlc; i++)
        printf("%d ", cfd.data[i]);
    
    printf("\n");
}
