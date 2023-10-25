#include "canbus_network.hpp"

namespace CanNetworkBase 
{
    CanNetwork::CanNetwork(std::string sInterface, int nBitrate)
    {
        this->m_nSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (this->m_nSocket < 0)
            throw CANException(SOCKET_ERR, "Error on socket define");

        strcpy(this->m_ifr.ifr_name, sInterface.c_str());
        ioctl(this->m_nSocket, SIOCGIFINDEX, &this->m_ifr);

        memset(&this->m_addr, 0, sizeof(this->m_addr));
        this->m_addr.can_family = AF_CAN;
        this->m_addr.can_ifindex = this->m_ifr.ifr_ifindex;

        if (bind(this->m_nSocket, (struct sockaddr *)&this->m_addr, sizeof(this->m_addr)) < 0)
            throw CANException(BINDING_ERR, "Error on socket association");
    }
}
