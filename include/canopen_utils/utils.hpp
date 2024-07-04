#include <cstdint>
#include <cstring>
#include <algorithm>

#include <linux/can.h>
#include <linux/can/raw.h>
#pragma once
namespace CANOpenUtils {

    #define CANOPEN_HEADER_SIZE 8
    #define CANOPEN_DATA_LENGTH 4

    enum MissionType
    {
        DOWNLOAD_REQ = 2,
        UPLOAD = 4,
        DOWNLOAD_REP = 6,
        ERROR = 8 
    };

    enum CanOpenPayloadByte 
    {
        CANOPEN_HEADER = 0,
        INDEX_LOW_BYTE,
        INDEX_HIGH_BYTE, 
        SUB_INDEX,
        PAYLOAD_0,
        PAYLOAD_1,
        PAYLOAD_2,
        PAYLOAD_3,

        TOTAL_PAYLOAD
    };

    typedef struct canopen_frame {

        uint32_t canopen_id;
        bool canopen_header[CANOPEN_HEADER_SIZE];
        uint16_t canopen_index;
        uint8_t canopen_subindex;
        uint8_t data[CANOPEN_DATA_LENGTH];

        canopen_frame& operator=(const canopen_frame& other) {
            if (this == &other)
                return *this;

            canopen_id = other.canopen_id;
            memcpy(&canopen_header, other.canopen_header, CANOPEN_HEADER_SIZE * sizeof(bool));
            canopen_index = other.canopen_index;
            canopen_subindex = other.canopen_subindex;
            memcpy(&data, other.data, CANOPEN_DATA_LENGTH * sizeof(uint8_t));

            return *this;
        }

    };

    void getCANOpenHeader(bool* pCanHeader, int nMissionType, int nPayloadLen);

    template <typename T>
    canopen_frame getFrameFromData(MissionType nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value)
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

    can_frame getCANBusFrameFromCANOpenFrame(canopen_frame coFrame);
    canopen_frame getCANOpenFramFromCANBusFrame(can_frame frame);

}