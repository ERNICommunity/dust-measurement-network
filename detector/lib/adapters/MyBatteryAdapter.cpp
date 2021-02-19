/*
 * MyBatteryAdapter.cpp
 *
 *  Created on: 15.03.2018
 *      Author: niklaudi
 */

#include <Arduino.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <MyBatteryAdapter.h>

MyBatteryAdapter::MyBatteryAdapter()
: BatteryAdapter()
, m_trPort(new DbgTrace_Port("batt", DbgTrace_Level::notice))
{ }

MyBatteryAdapter::~MyBatteryAdapter()
{
  delete m_trPort;
  m_trPort = 0;
}

unsigned int MyBatteryAdapter::readRawBattSenseValue()
{
#if defined (ARDUINO_ARCH_SAMD) && defined (__SAMD21G18A__) // Adafruit Feather M0
  #define VBATPIN A7
#else
  #define VBATPIN A5
#endif

  unsigned int rawBattSenseValue = analogRead(VBATPIN);
  return rawBattSenseValue;
}

void MyBatteryAdapter::notifyBattStateAnyChange()
{
  if ((0 != m_trPort) && (0 != battery()))
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::notice,
        "BatteryAdapter::notifyBattStateAnyChange(), %d.%02dV, %s",
        static_cast<int>(battery()->getBatteryVoltage()),
        static_cast<int>(battery()->getBatteryVoltage() * 100.0)
            - static_cast<int>(battery()->getBatteryVoltage()) * 100,
        battery()->isBattVoltageOk() ? "ok" : "/!\\");
  }
}
