#pragma once

// can bus include
#include <linux/can.h>
#include <linux/can/raw.h>

#include <sys/socket.h>

#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <string>
#include <exception>

namespace CanNetworkBase {

    class CanNetwork
    {
        private:

            int m_nSocket;
            struct ifreq m_ifr;
            struct sockaddr_can m_addr;

        public:

            CanNetwork(std::string sInterface, int nBitrate);
            virtual ~CanNetwork() {};

            int getSocket() { return this->m_nSocket; }
    };

    class CANException : public std::exception
    {
        private:
            int m_nStatusCode;
            std::string m_sMsg;

        public:
            CANException(int nStatusCode, std::string sMsg)
            {
                this->m_nStatusCode = nStatusCode;
                this->m_sMsg = sMsg;
            }

            int getStatusCode() { return m_nStatusCode; }
            std::string getMsg() { return m_sMsg; }
    };

    enum CanOpenTypeError
    {
        SOCKET_ERR = -1,
        BINDING_ERR = -2,

        WRITE_ON_SCK_ERR = -3,
        RECIVE_CAN_ERR = -4,
        MAX_LEN_EXCEEDED = -5,
        FAILED_RECV_RESP = -6
    };

};