#include "SystemStatusFacade.hpp"
#include <pb_encode.h>
#include <inttypes.h>
#include <SerialCommand.h>

SystemStatusFacade::SystemStatusFacade(LoraWanPriorityQueue* a_LoraWanInterface):
LoraWanDataHandler(a_LoraWanInterface),
m_ProtobufBatteryState()
{
    m_ProtobufBatteryState.voltage = 0.0f;
}

void SystemStatusFacade::setBatteryStatus(float a_Voltage)
{
    m_ProtobufBatteryState.voltage = a_Voltage;
    batteryState message = batteryState_init_zero;
    message = m_ProtobufBatteryState;

    uint8_t a_BufferRawData[128];
    uint64_t a_BufferLength;

    if(encode(&message, batteryState_fields, a_BufferRawData, &a_BufferLength)){
      if(m_LoraWanInterface!=NULL)
      {
        Serial.println("HELLO");
        m_LoraWanInterface->setHighPriorityPeriodicMessageData(a_BufferRawData,a_BufferLength);
      }
    }
}