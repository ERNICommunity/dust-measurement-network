
#ifndef _LoraDataHandler_SystemStatusFacade_h_
#define _LoraDataHandler_SystemStatusFacade_h_
#include <inttypes.h>
#include "LoraWanDataHandler.hpp"
#include "../Protobuf/batteryState.pb.h"

class SystemStatusFacade : LoraWanDataHandler
{
  public:
    SystemStatusFacade(LoraWanPriorityQueue* a_LoraWanInterface);
    void setBatteryStatus(float a_Voltage);
  private:
    batteryState m_ProtobufBatteryState;
};
#endif