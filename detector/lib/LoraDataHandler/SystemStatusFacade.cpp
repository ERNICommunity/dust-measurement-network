#include "SystemStatusFacade.hpp"
#include <pb_encode.h>
#include <inttypes.h>
#include <SerialCommand.h>

#include "../Protobuf/nodeStatus.pb.h"
SystemStatusFacade::SystemStatusFacade(LoraWanPriorityQueue* a_LoraWanInterface):
LoraWanDataHandler(a_LoraWanInterface)
{
}

void SystemStatusFacade::setBatteryStatus(State a_State, float a_Voltage)
{  
    protobuf_batteryState batteryState = protobuf_batteryState_init_zero;
    batteryState.state = m_ConstMapperState.at(a_State);
    batteryState.voltage = a_Voltage;
    protobuf_NodeMessage nodeMessage = protobuf_NodeMessage_init_zero;
    nodeMessage.information = protobuf_NodeMessage_InformationType_BATTERY_STATE;
    nodeMessage.which_msg = protobuf_NodeMessage_batteryStateData_tag;
    nodeMessage.msg.batteryStateData = batteryState;

    uint8_t a_BufferRawData[128];
    uint64_t a_BufferLength;

    if(encode(&nodeMessage, protobuf_NodeMessage_fields, a_BufferRawData, &a_BufferLength)){
      if(m_LoraWanInterface!=NULL)
      {
        m_LoraWanInterface->setHighPriorityPeriodicMessageData(a_BufferRawData,a_BufferLength);
      }
    }
}