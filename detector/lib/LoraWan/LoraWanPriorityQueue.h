#ifndef _LoraWan_LoraWanPriorityQueue_h_
#define _LoraWan_LoraWanPriorityQueue_h_
#undef max
#undef min
#include <inttypes.h>
#include <vector>

class LoRaWanDriver;

class LoraWanPriorityQueue
{
  public:
    explicit LoraWanPriorityQueue(LoRaWanDriver *a_LoraWandInterface);
    ~LoraWanPriorityQueue();
    void start();
    void setHighPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    void setLowPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    void setResponseMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    void setUpdateCycleHighPriorityPerdioc(uint64_t a_SendCycle);
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
