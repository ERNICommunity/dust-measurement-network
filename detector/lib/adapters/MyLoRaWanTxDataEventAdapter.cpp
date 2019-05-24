/*
 * MyLoRaWanTxDataEventAdapter.cpp
 *
 *  Created on: 06.03.2019
 *      Author: nid
 */

#include <SystemStatusFacade.h>
#include <MeasurementFacade.h>


#include <MyLoRaWanTxDataEventAdapter.h>

MyLoRaWanTxDataEventAdapter::MyLoRaWanTxDataEventAdapter(SystemStatusFacade* systemStatusFacade, MeasurementFacade* measurementFacade)
: m_systemStatusFacade(systemStatusFacade)
, m_measurementFacade(measurementFacade)
{ }

MyLoRaWanTxDataEventAdapter::~MyLoRaWanTxDataEventAdapter()
{ }

void MyLoRaWanTxDataEventAdapter::messageTransmitted()
{
  if (0 != m_systemStatusFacade)
  {
    m_systemStatusFacade->updateSystemStatus();
  }

  if (0 != m_measurementFacade)
  {
    m_measurementFacade->updateMeasurementData();
  }
}


