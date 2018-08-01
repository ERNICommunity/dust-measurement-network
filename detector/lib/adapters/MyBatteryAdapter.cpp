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
{ }

MyBatteryAdapter::~MyBatteryAdapter()
{ }

unsigned int MyBatteryAdapter::readRawBattSenseValue()
{
#if defined (ARDUINO_ARCH_SAMD) && defined (__SAMD21G18A__) // Adafruit Feather M0
  #define VBATPIN A7
#else
  #define VBATPIN A0
#endif

  unsigned int rawBattSenseValue = analogRead(VBATPIN);
  return rawBattSenseValue;
}
