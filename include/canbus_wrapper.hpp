#pragma once

// can bus include
#include <linux/can.h>
#include <linux/can/raw.h>

#include "canbus_network.hpp"

#include <thread>
#include <format>
#include <unistd.h>

namespace CanBusBase {

    class CanBusWrapper 
    {

        private:

            int m_nSocketCan;
            bool m_bStop = false;

            std::unique_ptr<std::thread> m_pThread;
            struct can_filter m_filter;

            void canBusCallback();

        public:

            CanBusWrapper(int nSocketCan, int nCanID = 0x00, int nCanMask = 0x00);
            void writeData(struct can_frame frame);
            virtual ~CanBusWrapper();

    };

}