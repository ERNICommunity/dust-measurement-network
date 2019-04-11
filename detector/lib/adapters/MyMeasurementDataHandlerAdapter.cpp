/*
 * MyMeasurementDataHandlerAdapter.cpp
 *
 *  Created on: 21.03.2019
 *      Author: nid
 */

#include <MeasurementFacade.h>
#include <PM_Process.h>
#include <DHT_Process.h>

#include <MyMeasurementDataHandlerAdapter.h>

MyMeasurementDataHandlerAdapter::MyMeasurementDataHandlerAdapter(MeasurementFacade* measurementFacade, PM_Process* pmProcess, DHT_Process* dhtProcess)
: IMeasurementDataAdapter()
, m_measurementFacade(measurementFacade)
, m_pmProcess(pmProcess)
, m_dhtProcess(dhtProcess)
{ }

MyMeasurementDataHandlerAdapter::~MyMeasurementDataHandlerAdapter()
{ }

void MyMeasurementDataHandlerAdapter::fetchMeasuremenData()
{
  if ((0 != m_measurementFacade) && (0 != m_pmProcess) && (0 != m_dhtProcess))
  {
    m_measurementFacade->setNewMeasurementData(m_pmProcess->getPm25Average(), m_pmProcess->getPm10Average(),
                                               m_dhtProcess->getRelHumidity(), m_dhtProcess->getTemperature());
  }
}
