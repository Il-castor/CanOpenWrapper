#include "utils.hpp"

void getCANOpenHeader(bool* pCanHeader, int nMissionType, int nPayloadLen)
{
    /**
     * @param nMissionType: it could by DOWNLOAD_REQ or UPLOAD
     * @param nPayloadLen: is the len of the Payload
     * 
     * @return bRet[CAN_OPEN_BYTE_0]: is the first byte of the CanBus Msg
    */
   switch (nMissionType)
    {
        case CanOpenWrapper::DOWNLOAD_REQ:

            pCanHeader[0] = false;
            pCanHeader[1] = false;
            pCanHeader[2] = true;
            pCanHeader[3] = false;

            for (int i = 5; i >= 4; i--)
                pCanHeader[i] = (uint8_t)nPayloadLen & (1 << CAN_MAX_DLEN - i - 3);
        
            pCanHeader[6] = 1;
            pCanHeader[7] = 1;

            break;

        case CanOpenWrapper::UPLOAD:

            pCanHeader[0] = false;
            pCanHeader[1] = true;
            pCanHeader[2] = false;
            pCanHeader[3] = false;

            for (int i = 4; i < CANOPEN_HEADER_SIZE; i++)
                pCanHeader[i] = nPayloadLen & (1 << i);
        
            break;

        default:
            return;
    
    }
}