/*
 * MyLoRaSysDataandlerAdapter.cpp
 *
 *  Created on: 01.03.2019
 *      Author: nid
 */

#include <Battery.h>
#include <MySystemStatusFacadeAdapter.h>

MySystemStatusFacadeAdapter::MySystemStatusFacadeAdapter(Battery* battery, SystemStatusFacade* systemStatusFacade)
: ISystemStatusFacadeAdapter()
, m_battery(battery)
, m_systemStatusFacade(systemStatusFacade)
{ }

MySystemStatusFacadeAdapter::~MySystemStatusFacadeAdapter()
{ }

SystemStatusFacade::State MySystemStatusFacadeAdapter::getBatteryState()
{
  SystemStatusFacade::State state = SystemStatusFacade::State::e_UNDEFINED;
  if (0 != m_battery)
  {
    if (m_battery->isBattVoltageOk())
    {
      state = SystemStatusFacade::State::e_OK;
    }
    if (m_battery->isBattVoltageBelowWarnThreshold())
    {
      state = SystemStatusFacade::State::e_WARNING;
    }
    if (m_battery->isBattVoltageBelowStopThreshold())
    {
      state = SystemStatusFacade::State::e_STOP;
    }
    if (m_battery->isBattVoltageBelowShutdownThreshold())
    {
      state = SystemStatusFacade::State::e_SHUTDOWN;
    }
    state = SystemStatusFacade::State::e_UNDEFINED;
  }
  return state;
}

float MySystemStatusFacadeAdapter::getBatteryVoltage()
{
  float voltage = 0.0;
  if (0 != m_battery)
  {
    voltage = m_battery->getBatteryVoltage();
  }
  return voltage;
}
