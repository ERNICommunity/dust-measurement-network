#include "MeasurementFacade.hpp"
#include <pb_encode.h>
#include <inttypes.h>
#include <SerialCommand.h>
#include "../Protobuf/dustMeasurement.pb.h"
#include "../Protobuf/nodeStatus.pb.h"

MeasurementFacade::MeasurementFacade(LoraWanPriorityQueue* a_LoraWanInterface):
LoraWanDataHandler(a_LoraWanInterface)
{
}

void MeasurementFacade::setNewMeasurementData(float a_ParticularMatter2_5, 
float a_ParticularMatter10,
float a_Temperature, 
float a_Humidity)
{
    dustSensorMeasurement dustSensor = dustSensorMeasurement_init_zero;
    dustSensor.particularMatter2_5um = a_ParticularMatter2_5;
    dustSensor.particularMatter10um = a_ParticularMatter10;
    dustSensor.temperature = a_Temperature;
    dustSensor.humidity = a_Humidity;

    NodeMessage nodeMessage = NodeMessage_init_zero;
    nodeMessage.information = NodeMessage_InformationType_DUST_MEASUREMENT;
    nodeMessage.which_msg = NodeMessage_dustMeasurementData_tag;
    nodeMessage.msg.dustMeasurementData = dustSensor;


    uint8_t a_BufferRawData[128];
    uint64_t a_BufferLength;

    if(encode(&nodeMessage, NodeMessage_fields, a_BufferRawData, &a_BufferLength)){
      if(m_LoraWanInterface!=NULL)
      {
        m_LoraWanInterface->setLowPriorityPeriodicMessageData(a_BufferRawData,a_BufferLength);
      }
    }
}