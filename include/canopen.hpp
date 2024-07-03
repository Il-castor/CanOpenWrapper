#include "canbus_wrapper.hpp"
#include "utils.hpp"

using namespace CanBusBase;

namespace CanOpenWrapper {

    class CANOpen
    {
        private:

            uint m_nNodeID, m_nBaseIDReq, m_nBaseIDResp;
            int m_nCounterCheck = 0;
            int m_nThreeshold = 0;

            CANOpenUtils::canopen_frame m_coLastMsgSent;
            CanBusWrapper* m_cCanWrapper;

            void init();

            template<typename Base, typename T>
            inline bool instanceof(const T) { return std::is_base_of<Base, T>::value; }

        public:

            CANOpen(uint nNodeID, CanBusWrapper* wrapper, uint nBaseIDReq, uint nBaseIDResp);
            void canBusListener(struct can_frame cfd);
            
            template <typename T>
            void download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut=5)
            {
                CANOpenUtils::canopen_frame coFrame;

                if ((this->instanceof<double>(value)) || (this->instanceof<long long int>(value)))
                    throw CanNetworkBase::CANException(CanNetworkBase::MAX_LEN_EXCEEDED, "[ CANOPEN ]: Maximum payload length exceeded");

                coFrame = CANOpenUtils::getFrameFromData<T>(CANOpenUtils::DOWNLOAD_REQ, nIndex, nSubIndex, value);
                coFrame.canopen_id = this->m_nBaseIDReq + m_nNodeID;

                can_frame frame = CANOpenUtils::getCANBusFrameFromCANOpenFrame(coFrame);

                // Send the frame on the CANBus
                this->m_cCanWrapper->writeData(frame);

                this->m_coLastMsgSent = coFrame;
            }
            
            //casta a float gli ultimi 4 byte tramite template 
            template <typename T>
            T upload(uint16_t nIndex, uint8_t nSubIndex, int nTimeOut=5)
            {
                CANOpenUtils::canopen_frame coFrame;

            
                coFrame = CANOpenUtils::getFrameFromData<T>(CANOpenUtils::UPLOAD, nIndex, nSubIndex);

                can_frame cFrame = this->m_cCanWrapper->readData(coFrame);

                //can_frame frame = CANOpenUtils::getCANBusFrameFromCANOpenFrame(coFrame);
                CANOpenUtils frame =  CANOpenUtils::getCANOpenFramFromCANBusFrame(cFrame);

                /*frame.canopen_id = this->m_nBaseIDReq + m_nNodeID;
                frame.canopen_index 
                frame.canopen_subindex 
                frame.canopen_data */



                // dobbiamo leggere, prima parte uguale all'altra a parte la richiesta 
                // controllare messaggio di ritorno, di tipo can_frame, dopo lo devi interpretare 
                T data; 
                //memcpy(&data, &c_frame.data[c_frame.can_dlc -4], sizeof(T));

                //return data;


            }
            
    };

}