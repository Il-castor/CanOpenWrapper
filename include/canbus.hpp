#include "canbus_wrapper.hpp"

class CANBus : public CanBusBase::CanBusWrapper
{
    public:

        CANBus(int nSocketCan, int nCanID = 0x00, int nCanMask = 0x00);
        void canBusListener(struct can_frame cfd);
};