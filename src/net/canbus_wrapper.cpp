#include "canbus_wrapper.hpp"

using namespace CanBusBase;
using namespace CanNetworkBase;

CanBusWrapper::CanBusWrapper(int nSocketCan, int nCanID, int nCanMask)
{
    this->m_nSocketCan = nSocketCan;
    this->m_pThread = std::make_unique<std::thread>([this]() { canBusCallback(); });
}

void CanBusWrapper::canBusCallback()
{
    while (!this->m_bStop)
    {
        struct can_frame cfd;

        int nBytes = read(this->m_nSocketCan, &cfd, sizeof(struct can_frame));
        if (nBytes > 0)
        {

            for (Subscription& s : this->m_vSubscriptions)
            {
                if ((cfd.can_id & s.m_filter.can_mask) == (s.m_filter.can_id & s.m_filter.can_mask))
                {
                    try {
                        s.callback(cfd);
                    }
                    catch (CANException &eErr) {}
                }
            }
        }

    }
}

void CanBusWrapper::subscribe(struct can_filter filter, callback_t callback)
{
    Subscription sub = {filter, callback};
    this->m_vSubscriptions.push_back(sub);
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