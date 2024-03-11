#pragma once

#include "canbus_network.hpp"

#include <thread>
#include <format>
#include <unistd.h>
#include <functional>
#include <vector>
#include <memory>

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

            void canBusCallback();
        
        public:

            CanBusWrapper(int nSocketCan, int nCanID, int nCanMask);
            
            void writeData(struct can_frame frame);
            void subscribe(struct can_filter filter, callback_t callback);
            
            ~CanBusWrapper();

    };

}