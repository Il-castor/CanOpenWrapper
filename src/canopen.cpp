#include "canopen.hpp"

namespace CanOpenWrapper 
{

    CANOpen::CANOpen(int nNodeID, int nSocketCan, int nBaseID)
    : CanBusBase::CanBusWrapper(nSocketCan, nBaseID + nNodeID, 0x7FF)
    {
        this->m_nNodeID = nNodeID;
        this->m_nBaseID = nBaseID;
        this->init();
    }

    void CANOpen::init()
    {
        can_frame frame;

        frame.can_id = 0x0;
        frame.can_dlc = 2;
        memset(&frame, 0x01, 2);

        this->writeData(frame);
    }

}