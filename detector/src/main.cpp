/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <Timer.h>          // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer

// private libraries
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <AppDebug.h>
#include <ProductDebug.h>
#include <RamUtils.h>
#include <Battery.h>
#include <MyBatteryAdapter.h>

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
Battery* battery = 0;

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();
  //-----------------------------------------------------------------------------
  // Battery Voltage Surveillance
  //-----------------------------------------------------------------------------
  BatteryThresholdConfig battCfg = { 3.6, // BATT_WARN_THRSHD [V]
                                     3.4, // BATT_STOP_THRSHD [V]
                                     3.2, // BATT_SHUT_THRSHD [V]
                                     0.1  // BATT_HYST        [V]
                                    };
  battery = new Battery(new MyBatteryAdapter(), battCfg);
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  yield();                  // process Timers
}
