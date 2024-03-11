#include "canopen.hpp"

namespace CanOpenWrapper 
{

    CANOpen::CANOpen(int nNodeID, std::shared_ptr<CanBusWrapper> wrapper, int nBaseIDReq, int nBaseIDResp) : m_cCanWrapper(wrapper)
    {
        this->m_nNodeID = nNodeID;
        this->m_nBaseIDReq = nBaseIDReq;
        this->m_nBaseIDResp = nBaseIDResp;
        
        this->init();
    }

    void CANOpen::init()
    {
        can_frame frame;

        frame.can_id = 0x00;
        frame.can_dlc = 2;
        memset(&frame.data, 0x01, 2);

        this->m_cCanWrapper->writeData(frame);
    }

    void CANOpen::canBusListener(struct can_frame cfd)
    {
        CANOpenUtils::canopen_frame coFrame;
        coFrame = CANOpenUtils::getCANOpenFramFromCANBusFrame(cfd);

        if (this->m_coLastMsgSent.canopen_id != 0x00)
        {
            if (this->m_nThreeshold < this->m_nCounterCheck)
            {
                if ((this->m_coLastMsgSent.canopen_index == coFrame.canopen_index) &&
                    (this->m_coLastMsgSent.canopen_subindex == coFrame.canopen_subindex))
                {
                    memset(&this->m_coLastMsgSent, '\0', sizeof(CANOpenUtils::canopen_frame));
                    this->m_nCounterCheck = 0;
                }
                else { this->m_nCounterCheck++; }
            }
            else 
            {
                memset(&this->m_coLastMsgSent, '\0', sizeof(CANOpenUtils::canopen_frame));
                this->m_nCounterCheck = 0;
                throw CanNetworkBase::CANException(CanNetworkBase::FAILED_RECV_RESP, "Failed to Read the Response from CanBus");
            }
        }

        // TODO: implement for only download, check if on the upload could it be
        // some error introduct for the continuos request, maybe FIFO Queue
    }

}