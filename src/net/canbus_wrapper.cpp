#include "canbus_wrapper.hpp"
// #include "utils.hpp"

using namespace CanBusBase;
using namespace CanNetworkBase;

CanBusWrapper::CanBusWrapper(int nSocketCan, int nCanID, int nCanMask)
   : m_pThread([this]() { canBusCallback(); }), m_nSocketCan(nSocketCan) {}

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
    std::unique_lock<std::mutex> lock(this->m_mAddSubscribe);
    this->m_vSubscriptions.emplace_back(filter, callback);
}



void CanBusWrapper::writeData(struct can_frame frame)
{
    std::unique_lock<std::mutex> lock(this->m_mWriteOnSocket);
    if (write(this->m_nSocketCan, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        throw CANException(WRITE_ON_SCK_ERR, "Error on write data to socket");
}

can_frame CanBusWrapper::readData(CANOpenUtils::canopen_frame frame)
{
    // ELE Forse c'e da aggiungere un mutex uno del tipo read on socket ?
    std::unique_lock<std::mutex> lock(this->m_mWriteOnSocket);  
    if (read(this->m_nSocketCan, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        throw CANException(READ_ON_SCK_ERR, "Error on read data from socket");
    // ELE return ?? 
    // return frame;
}

CanBusWrapper::~CanBusWrapper()
{
    this->m_bStop = true;
   if (this->m_pThread.joinable())
       this->m_pThread.join();
}