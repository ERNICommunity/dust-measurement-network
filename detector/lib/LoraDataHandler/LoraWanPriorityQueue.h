#ifndef _LoraWan_LoraWanPriorityQueue_h_
#define _LoraWan_LoraWanPriorityQueue_h_
#undef max
#undef min
#include <inttypes.h>
#include <vector>

class LoRaWanDriver;

/**
 * LoRaWan Priority Queue
 *
 *
 */
class LoraWanPriorityQueue
{
  public:
    explicit LoraWanPriorityQueue(LoRaWanDriver *a_LoraWandInterface);
    ~LoraWanPriorityQueue();
    void start();

    /**
     * Set data to be send periodically with high priority
     *
     * @param data Pointer to the data byte stream
     * @param sizeOfData Number of bytes to be sent
     */
    void setHighPriorityPeriodicMessageData(uint8_t* data, uint64_t sizeOfData);

    /**
     * Set data to be send periodically with low priority
     *
     * @param data Pointer to the data byte stream
     * @param sizeOfData Number of bytes to be sent
     */
    void setLowPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);

    /**
     * Set response data to be send
     *
     * @param data Pointer to the data byte stream
     * @param sizeOfData Number of bytes to be sent
     */
    void setResponseMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);

    /**
     * Set the ratio between low prio and hiegh prio messages sent
     * I.e. sendCycle = 2: 2 low prio messages followed by 1 high prio message are going out.
     * @param sendCycle Ratio between low prio and hiegh prio messages sent
     */
    void setUpdateCycleHighPriorityPerdioc(uint64_t sendCycle);

    /**
     * Update
     */
    void update();

  private:
    LoRaWanDriver *m_LoraWandInterface;
    
    enum class TypeOfMessage{
      e_Undefined,
      e_PeriodicLowPrio,
      e_PeriodicHighPrio,
      e_ResponseMessage
    };
    TypeOfMessage m_CurrentTypeOfeMessage;
    uint8_t m_SentCounterCurrentMessage;
    uint64_t m_UpdateCycleHighPriority;
    std::vector<uint8_t> *m_BufferResponse;
    std::vector<uint8_t> *m_BufferHighPrio;
    std::vector<uint8_t> *m_BufferLowPrio;

    bool m_ForceUpdateBuffer;
    uint64_t m_counterLowPriority;
    void updateLoraSentMessage();
    void setNextLoraSentMessage(const TypeOfMessage& a_NextMessage);

};
#endif
