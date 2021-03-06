#include <cstddef>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <LoraWanPriorityQueue.h>
#include <LoRaWanDriver.h>


LoraWanPriorityQueue::LoraWanPriorityQueue(LoRaWanDriver* a_LoraWandInterface)
: m_LoraWandInterface(a_LoraWandInterface)
, m_CurrentTypeOfMessage(TypeOfMessage::e_Undefined)
, m_UpdateCycleHighPriority(0)
, m_BufferResponse(0)
, m_BufferHighPrio(0)
, m_BufferLowPrio(0)
, m_ForceUpdateBuffer(false)
, m_counterLowPriority(0)
{ }


LoraWanPriorityQueue::~LoraWanPriorityQueue()
{
  if (m_BufferResponse != 0)
  {
    delete m_BufferResponse;
    m_BufferResponse = 0;
  }
  if (m_BufferHighPrio != 0)
  {
    delete m_BufferHighPrio;
    m_BufferHighPrio = 0;
  }
  if (m_BufferLowPrio != 0)
  {
    delete m_BufferLowPrio;
    m_BufferLowPrio = 0;
  }
}

void LoraWanPriorityQueue::start()
{
  m_CurrentTypeOfMessage = TypeOfMessage::e_PeriodicLowPrio;
  m_ForceUpdateBuffer = true;
  m_counterLowPriority = 0;
}

void LoraWanPriorityQueue::setResponseMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{
  if (m_BufferResponse != 0)
  {
    delete m_BufferResponse;
  }
  m_BufferResponse = new std::vector<uint8_t>(a_Data, a_Data + a_SizeOfData);
  setNextLoraSentMessage(TypeOfMessage::e_ResponseMessage);
}

void LoraWanPriorityQueue::setLowPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{
  if (m_BufferLowPrio != 0)
  {
    delete m_BufferLowPrio;
    m_BufferLowPrio = 0;
  }
  m_BufferLowPrio = new std::vector<uint8_t>(a_Data, a_Data + a_SizeOfData);
}

void LoraWanPriorityQueue::setHighPriorityPeriodicMessageData(uint8_t* data, uint64_t sizeOfData)
{
  if (m_BufferHighPrio != 0)
  {
    delete m_BufferHighPrio;
  }
  m_BufferHighPrio = new std::vector<uint8_t>(data, data + sizeOfData);
}

void LoraWanPriorityQueue::setUpdateCycleHighPriorityPerdioc(uint64_t sendCycle)
{
  if (sendCycle > 0)
  {
    m_UpdateCycleHighPriority = sendCycle;
  }
  else
  {
    //TODO Throw Error
  }
}

void LoraWanPriorityQueue::update()
{
  //m_SentCounterCurrentMessage = m_LoraWandInterface->getSentCounterPeriodicMessage();
  updateLoraSentMessage();
}

void LoraWanPriorityQueue::updateLoraSentMessage()
{
  TypeOfMessage nextMessage = TypeOfMessage::e_Undefined;
  uint64_t counterCurrentMessage = m_LoraWandInterface->getSentCounterPeriodicMessage();

  if (counterCurrentMessage > 0 || m_ForceUpdateBuffer)
  {
    switch (m_CurrentTypeOfMessage)
    {
      case TypeOfMessage::e_Undefined:
        break;
      case TypeOfMessage::e_ResponseMessage:
        if (counterCurrentMessage > 0)
        {
          nextMessage = TypeOfMessage::e_PeriodicLowPrio;
        }
        break;
      case TypeOfMessage::e_PeriodicHighPrio:
        nextMessage = TypeOfMessage::e_PeriodicLowPrio;
        break;
      case TypeOfMessage::e_PeriodicLowPrio:
        if ((m_counterLowPriority >= m_UpdateCycleHighPriority)
            && (m_UpdateCycleHighPriority > 0))
        {
          nextMessage = TypeOfMessage::e_PeriodicHighPrio;
          m_counterLowPriority = 0;
        }
        else
        {
          m_counterLowPriority++;
          nextMessage = TypeOfMessage::e_PeriodicLowPrio;
        }
        break;
      default:
        break;
    }
    if (nextMessage != TypeOfMessage::e_Undefined)
    {
      m_ForceUpdateBuffer = false;
      setNextLoraSentMessage(nextMessage);
    }
  }
}

void LoraWanPriorityQueue::setNextLoraSentMessage(const TypeOfMessage& a_NextMessage)
{
  uint8_t* data;
  uint64_t sizeOfData = 0;
  switch (a_NextMessage)
  {
    case TypeOfMessage::e_Undefined:
      break;
    case TypeOfMessage::e_ResponseMessage:
      if (m_BufferResponse != 0)
      {
        sizeOfData = m_BufferResponse->size();
        data = &(*m_BufferResponse)[0];
      }
      break;
    case TypeOfMessage::e_PeriodicHighPrio:
      if (m_BufferHighPrio != 0)
      {
        sizeOfData = m_BufferHighPrio->size();
        data = &(*m_BufferHighPrio)[0];

        if (0 != m_LoraWandInterface)
        {
          char singleBuf[5];
          char strBuf[20 * sizeOfData];
          strcpy(strBuf, "");
          for (unsigned int i = 0; i < sizeOfData; i++)
          {
            sprintf(singleBuf, "%02X%s", data[i],
                (i != sizeOfData - 1) ? " " : "");
            strcat(strBuf, singleBuf);
          }
          TR_PRINTF(m_LoraWandInterface->trPort(), DbgTrace_Level::info,
              "PriorityQueue setNextLoraSentMessage() - HighPrio: %s", strBuf);
        }
      }
      break;
    case TypeOfMessage::e_PeriodicLowPrio:
      if (m_BufferLowPrio != 0)
      {
        sizeOfData = m_BufferLowPrio->size();
        data = &(*m_BufferLowPrio)[0];

        if (0 != m_LoraWandInterface)
        {
          char singleBuf[5];
          char strBuf[20 * sizeOfData];
          strcpy(strBuf, "");
          for (unsigned int i = 0; i < sizeOfData; i++)
          {
            sprintf(singleBuf, "%02X%s", data[i],
                (i != sizeOfData - 1) ? " " : "");
            strcat(strBuf, singleBuf);
          }
          TR_PRINTF(m_LoraWandInterface->trPort(), DbgTrace_Level::info,
              "PriorityQueue setNextLoraSentMessage() - Low Prio: %s", strBuf);
        }
      }
      break;
    default:
      break;
  }
  if (sizeOfData > 0)
  {
    m_LoraWandInterface->setPeriodicMessageData(data, sizeOfData);
  }
}
