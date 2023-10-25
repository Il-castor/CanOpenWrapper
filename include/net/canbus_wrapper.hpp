#pragma once

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
        
        protected:

            virtual void canBusListener(struct can_frame cfd) = 0;

        public:

            CanBusWrapper(int nSocketCan, int nCanID, int nCanMask);
            void writeData(struct can_frame frame);
            virtual ~CanBusWrapper();

    };

}