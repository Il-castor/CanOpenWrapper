#include "canbus_wrapper.hpp"

using namespace CanBusBase;

class CANBus
{
    private:

        CanBusWrapper* m_cCanWrapper;

    public:

        CANBus(int nSocketCan, CanBusWrapper* wrapper, uint nMsgID=0x0, uint nMask=0x0);
        void canBusListener(struct can_frame cfd);
};