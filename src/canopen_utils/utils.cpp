#include "utils.hpp"

namespace CANOpenUtils
{

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
            case DOWNLOAD_REQ:

                pCanHeader[0] = false;
                pCanHeader[1] = false;
                pCanHeader[2] = true;
                pCanHeader[3] = false;

                for (int i = 5; i >= 4; i--)
                    pCanHeader[i] = (uint8_t)nPayloadLen & (1 << CAN_MAX_DLEN - i - 3);
            
                pCanHeader[6] = 1;
                pCanHeader[7] = 1;

                break;

            case UPLOAD:

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

    template <typename T>
    canopen_frame getFrameFromData(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value)
    {
        canopen_frame frame;

        // In the First Byte of the Payload: 
        // n is equal to the number of byte discarded starting from seventh byte
        int nLen = CANOPEN_DATA_LENGTH - sizeof(T);

        // Configuration of the first Header Byte
        getCANOpenHeader(&frame.canopen_header[0], nMsgType, nLen);

        // Set Index of the CanOpenFrame
        frame.canopen_index = nIndex;

        // Set SubIndex of the CanOpenFrame
        frame.canopen_subindex = nSubIndex;

        std::fill(frame.data, frame.data + sizeof(int), 0);
        memcpy(frame.data, &value, sizeof(T));

        // Insert Padding for the CANBus msg
        int nFree = 0;
        for (int i = 0; i < sizeof(T); i++)
        {
            if (frame.data[i] == 0)
                nFree += 1;
            else
                break;
        }

        uint8_t correction[CANOPEN_DATA_LENGTH];
        if (nFree > 0)
        {
            memcpy(correction, frame.data + nFree, sizeof(T) - nFree);
            memcpy(correction + sizeof(T) - nFree, frame.data, nFree);
            memcpy(frame.data, correction, sizeof(T));
        }

        return frame;
    }
    
    can_frame getCANBusFrameFromCANOpenFrame(canopen_frame coFrame)
    {
        can_frame frame;
        int j = CAN_MAX_DLEN - 1;

        frame.can_id = coFrame.canopen_id;
        frame.can_dlc = CAN_MAX_DLEN;
        memset(frame.data, '\0', CAN_MAX_DLEN);

        for (int i = 0; i < CANOPEN_HEADER_SIZE; i++)
        {
            if (coFrame.canopen_header[i])
                frame.data[CANOPEN_HEADER] |= 1 << (CANOPEN_HEADER_SIZE - i - 1);
        }

        frame.data[INDEX_LOW_BYTE] = (uint8_t)coFrame.canopen_index;
        frame.data[INDEX_HIGH_BYTE] = (uint8_t)(coFrame.canopen_index >> 8);
        frame.data[SUB_INDEX] = coFrame.canopen_subindex;
        for (int i = CANOPEN_DATA_LENGTH - 1; i >= 0; i--)
        {
            frame.data[j] = coFrame.data[i];
            j --;
        }

        return frame;
    }

    canopen_frame getCANOpenFramFromCANBusFrame(can_frame frame)
    {
        canopen_frame coFrame;

        coFrame.canopen_id = frame.can_id;
        for (int i = 0; i < CANOPEN_HEADER_SIZE; i++)
            coFrame.canopen_header[i] = (frame.data[CANOPEN_HEADER] >> i) & 1;
        coFrame.canopen_index = frame.data[INDEX_HIGH_BYTE] << 8 | frame.data[INDEX_LOW_BYTE];
        coFrame.canopen_subindex = frame.data[SUB_INDEX];

        int j = 8;
        for (int i = PAYLOAD_0; i < TOTAL_PAYLOAD; i++)
        {
            coFrame.data[i] = frame.data[j];
            j --;
        }

        return coFrame;
    }

}