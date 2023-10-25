#include <cstdint>
#include <cstring>
#include <algorithm>

#include <linux/can.h>
#include <linux/can/raw.h>

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

    };

    void getCANOpenHeader(bool* pCanHeader, int nMissionType, int nPayloadLen);

    template <typename T>
    canopen_frame getFrameFromData(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value);

    can_frame getCANBusFrameFromCANOpenFrame(canopen_frame coFrame);
    canopen_frame getCANOpenFramFromCANBusFrame(can_frame frame);

}