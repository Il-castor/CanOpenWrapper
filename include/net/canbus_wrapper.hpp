#pragma once

#include "canbus_network.hpp"
#include "canopen_utils/utils.hpp"


#include <thread>
#include <unistd.h>
#include <functional>
#include <vector>
#include <memory>
#include <mutex>

namespace CanBusBase {

    using callback_t = std::function<void(struct can_frame)>;

    class CanBusWrapper 
    {
        struct Subscription {
            
            can_filter m_filter;
            callback_t callback;

            Subscription(can_filter f, callback_t c) : m_filter(f), callback(c) {}

        };

        private:

            std::thread m_pThread;
            int m_nSocketCan;
            bool m_bStop = false;

            std::vector<Subscription> m_vSubscriptions;
            std::mutex m_mAddSubscribe;
            std::mutex m_mWriteOnSocket;

            void canBusCallback();
        
        public:

            CanBusWrapper(int nSocketCan, int nCanID, int nCanMask);
            
            void writeData(struct can_frame frame);
            void subscribe(struct can_filter filter, callback_t callback);

            

            can_frame readData(CANOpenUtils::canopen_frame frame);

            ~CanBusWrapper();

    };

}