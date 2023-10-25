#include "canbus_wrapper.hpp"

using namespace CanBusBase;
using namespace CanNetworkBase;

CanBusWrapper::CanBusWrapper(int nSocketCan, int nCanID, int nCanMask)
{
    this->m_nSocketCan = nSocketCan;
    this->m_pThread = std::make_unique<std::thread>([this]() { canBusCallback(); });

    this->m_filter.can_id = nCanID;
    this->m_filter.can_mask = nCanMask;
}

void CanBusWrapper::canBusCallback()
{
    while (!this->m_bStop)
    {
        struct can_frame cfd;

        int nBytes = read(this->m_nSocketCan, &cfd, sizeof(struct can_frame));
        if (nBytes > 0)
        {
            if ((cfd.can_id & this->m_filter.can_mask) == (this->m_filter.can_id & this->m_filter.can_mask))
            {
                /* if it need it possible to implement everything */
                try {
                    this->canBusListener(cfd);
                }
                catch (CANException &eErr) {}
            }
        }

    }
}

void CanBusWrapper::writeData(struct can_frame frame)
{
    if (write(this->m_nSocketCan, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        throw CANException(WRITE_ON_SCK_ERR, "Error on write data to socket");
}

CanBusWrapper::~CanBusWrapper()
{
    this->m_bStop = true;
    if (this->m_pThread && this->m_pThread->joinable())
        this->m_pThread->join();
}