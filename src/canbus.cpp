#include "canbus.hpp"


CANBus::CANBus(int nSocketCan, CanBusWrapper* wrapper, uint nMsgID, uint nMask)
: m_cCanWrapper(wrapper) 
{
    this->m_cCanWrapper->subscribe(
        {nMsgID, nMask}, 
        std::bind(&CANBus::canBusListener, this, std::placeholders::_1)
    );
}

void CANBus::canBusListener(struct can_frame cfd) 
{
    printf("[ ID ]: %d, [ DLC ]: %d, [ DATA ]: ", cfd.can_id, cfd.can_dlc);
    for (int i = 0; i < cfd.can_dlc; i++)
        printf("%d ", cfd.data[i]);
    
    printf("\n");
}
