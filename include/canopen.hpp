#include "canbus_wrapper.hpp"

namespace CanOpenWrapper {

    #define CANOPEN_HEADER_SIZE 8
    #define CANOPEN_DATA_LENGTH 4

    enum MissionType
    {
        DOWNLOAD_REQ = 2,
        UPLOAD = 4,
        DOWNLOAD_REP = 6,
        ERROR = 8 
    };

    typedef struct canopen_frame {

        uint32_t canopen_id;
        bool canopen_header[CANOPEN_HEADER_SIZE];
        uint16_t canopen_index;
        uint8_t canopen_subindex;
        uint8_t data[CANOPEN_DATA_LENGTH];

    };

    class CANOpen : public CanBusBase::CanBusWrapper
    {
        private:

            int m_nNodeID;
            int m_nBaseID;
            void init();

            template<typename Base, typename T>
            inline bool instanceof(const T) { return std::is_base_of<Base, T>::value; }

        public:

            CANOpen(int nNodeID, int nSocketCan, int nBaseID);
            
            template <typename T>
            canopen_frame download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut=5)
            {
                canopen_frame frame;
                canopen_frame retFrame;

                if ((this->instanceof<double>(value)) || (this->instanceof<long long int>(value)))
                    throw CanNetworkBase::CANException(CanNetworkBase::MAX_LEN_EXCEEDED, "[ CANOPEN ]: Maximum payload length exceeded");
            }
    };

}