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

    void printCanopenFrame(const canopen_frame& frame) {
        printf("CAN open frame\tid: %x\t", frame.canopen_id );
        
        std::cout << "header: ";
        for (size_t i = 0; i < CANOPEN_HEADER_SIZE; ++i) {
            std::cout << frame.canopen_header[i] << " ";
        }
        std::cout << "\t";
        
        printf("index: 0x%x \t" , frame.canopen_index );
        printf("subindex: 0x%x \t" , frame.canopen_subindex );
        
        std::cout << "data: ";
        for (size_t i = 0; i < CANOPEN_DATA_LENGTH; ++i) {
            printf("0x%x ", frame.data[i]);
        }
        std::cout << std::endl;
    }

    void printCanFrame(const can_frame& frame) {
        printf("CAN frame     \tid: %x \t dlc: %d\t data: ", frame.can_id, frame.can_dlc);
        for (int i=0; i<CAN_MAX_DLEN; i++)
            printf("0x%x ", frame.data[i]);
        printf("\n");
    
    }




}