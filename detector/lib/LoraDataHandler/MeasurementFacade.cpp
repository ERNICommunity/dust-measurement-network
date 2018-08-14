#include "MeasurementFacade.hpp"
#include <pb_encode.h>
#include <inttypes.h>
#include <SerialCommand.h>

MeasurementFacade::MeasurementFacade(LoraWanPriorityQueue* a_LoraWanInterface):
LoraWanDataHandler(a_LoraWanInterface),
m_ProtobufData()
{
 m_ProtobufData.humidity = 0.0f;
 m_ProtobufData.particularMatter10um = 0.0f;
 m_ProtobufData.particularMatter2_5um = 0.0f;
 m_ProtobufData.temperature = 0.0f;
}

void MeasurementFacade::setNewMeasurementData(float a_ParticularMatter2_5, 
float a_ParticularMatter10,
float a_Temperature, 
float a_Humidity)
{
    m_ProtobufData.humidity = a_Humidity;
    m_ProtobufData.temperature = a_Temperature;
    m_ProtobufData.particularMatter10um = a_ParticularMatter10;
    m_ProtobufData.particularMatter2_5um = a_ParticularMatter2_5;
    dustSensorMeasurement message = dustSensorMeasurement_init_zero;
    message = m_ProtobufData;

    uint8_t a_BufferRawData[128];
    uint64_t a_BufferLength;

    if(encode(&message, dustSensorMeasurement_fields, a_BufferRawData, &a_BufferLength)){
      if(m_LoraWanInterface!=NULL)
      {
        m_LoraWanInterface->setLowPriorityPeriodicMessageData(a_BufferRawData,a_BufferLength);
      }
    }
}