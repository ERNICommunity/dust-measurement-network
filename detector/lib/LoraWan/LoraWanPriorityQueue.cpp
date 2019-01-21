#include "LoraWanPriorityQueue.hpp"
#include <cstddef>
#include <SerialCommand.h>

LoraWanPriorityQueue::LoraWanPriorityQueue(LoRaWanDriver *a_LoraWandInterface):
m_LoraWandInterface(a_LoraWandInterface),
m_CurrentTypeOfeMessage(TypeOfMessage::e_Undefined),
m_SentCounterCurrentMessage(0),
m_UpdateCycleHighPriority(0),
m_BufferResponse(NULL),
m_BufferHighPrio(NULL),
m_BufferLowPrio(NULL),
m_ForceUpdateBuffer(false),
m_counterLowPriority(0)
{
}


LoraWanPriorityQueue::~LoraWanPriorityQueue(){
    if(m_BufferResponse!=NULL){
        delete m_BufferResponse;
    }
    if(m_BufferHighPrio!=NULL){
        delete m_BufferHighPrio;
    }
    if(m_BufferLowPrio!=NULL){
        delete m_BufferLowPrio;
    }
}

void LoraWanPriorityQueue::start()
{
    m_CurrentTypeOfeMessage = TypeOfMessage::e_PeriodicLowPrio;
    m_ForceUpdateBuffer = true;
    m_counterLowPriority = 0;
}

void LoraWanPriorityQueue::setResponseMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{
    if(m_BufferResponse!=NULL){
        delete m_BufferResponse;
    }
    m_BufferResponse = new std::vector<uint8_t>(a_Data,a_Data+a_SizeOfData);
    setNextLoraSentMessage(TypeOfMessage::e_ResponseMessage);
}

void LoraWanPriorityQueue::setLowPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData){
    if(m_BufferLowPrio!=NULL){
        delete m_BufferLowPrio;
    }
    m_BufferLowPrio = new std::vector<uint8_t>(a_Data,a_Data+a_SizeOfData);
}

void LoraWanPriorityQueue::setHighPriorityPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData){
    if(m_BufferHighPrio!=NULL){
        delete m_BufferHighPrio;
    }
    m_BufferHighPrio = new std::vector<uint8_t>(a_Data,a_Data+a_SizeOfData);
}

void LoraWanPriorityQueue::setUpdateCycleHighPriorityPerdioc(uint64_t a_SendCycle)
{
    if(a_SendCycle>0){
        m_UpdateCycleHighPriority = a_SendCycle;
    }
    else{
        //TODO Throw Error
    }
}


void LoraWanPriorityQueue::update(){
 //m_SentCounterCurrentMessage = m_LoraWandInterface->getSentCounterPeriodicMessage();
    updateLoraSentMessage();
}

void LoraWanPriorityQueue::updateLoraSentMessage()
{
    TypeOfMessage nextMessage = TypeOfMessage::e_Undefined;
    uint64_t counterCurrentMessage = m_LoraWandInterface->getSentCounterPeriodicMessage();
    
    if(counterCurrentMessage>0||m_ForceUpdateBuffer)
    {
        switch(m_CurrentTypeOfeMessage){
            case TypeOfMessage::e_Undefined:
            break;
            case TypeOfMessage::e_ResponseMessage:
                if(counterCurrentMessage>0)
                {
                    nextMessage = TypeOfMessage::e_PeriodicLowPrio;
                }
            break;
            case TypeOfMessage::e_PeriodicHighPrio:
                    nextMessage = TypeOfMessage::e_PeriodicLowPrio;
            break;
            case TypeOfMessage::e_PeriodicLowPrio:
                if((m_counterLowPriority>=m_UpdateCycleHighPriority)&&
                    (m_UpdateCycleHighPriority>0))
                {
                    nextMessage = TypeOfMessage::e_PeriodicHighPrio;
                    m_counterLowPriority = 0;
                }
                else{
                    m_counterLowPriority++;
                    nextMessage = TypeOfMessage::e_PeriodicLowPrio;
                }
            break;
            default:
            break;
        }
        if(nextMessage!=TypeOfMessage::e_Undefined)
        {
            m_ForceUpdateBuffer=false;
            setNextLoraSentMessage(nextMessage);
        }
    }
}

void LoraWanPriorityQueue::setNextLoraSentMessage(const TypeOfMessage& a_NextMessage)
{
     uint8_t* data;
     uint64_t sizeOfData = 0;
     switch(a_NextMessage){
        case TypeOfMessage::e_Undefined:
        break;
        case TypeOfMessage::e_ResponseMessage:
            if(m_BufferResponse!=NULL)
            {
            sizeOfData = m_BufferResponse->size();
            data = &(*m_BufferResponse)[0];
            }
        break;
        case TypeOfMessage::e_PeriodicHighPrio:
            if(m_BufferHighPrio!=NULL)
            {
            sizeOfData = m_BufferHighPrio->size();
            data = &(*m_BufferHighPrio)[0];
            for(int i=0;i<sizeOfData;i++)
            {
                Serial.print((int)data[i]);
                Serial.print(" ");
            }
            }
        break;
        case TypeOfMessage::e_PeriodicLowPrio:
            if(m_BufferLowPrio!=NULL)
            {
            sizeOfData = m_BufferLowPrio->size();
            data = &(*m_BufferLowPrio)[0];
            }
        break;
        default:
        break;
    }
    if(sizeOfData>0)
    {
        m_LoraWandInterface->setPeriodicMessageData(data,sizeOfData);
    }
}
