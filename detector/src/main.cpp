/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>
#undef max
#undef min

// PlatformIO libraries
#include <SerialCommand.h>    // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <Timer.h>            // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer
#include <Adafruit_Sensor.h>  // pio lib 19, 31, lib details see https://github.com/adafruit/DHT-sensor-library
#include <DHT.h>
#include <DHT_U.h>

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
#include <PM_Process.h>
#include <MyPM_ProcessAdapter.h>
#include <DHT_Process.h>
#include <MyDHT_ProcessAdapter.h>
#include <LoraWanAbp.hpp>
#include <LoraWanAdapter.hpp>
#include <LoraWanPriorityQueue.hpp>
#include <MeasurementFacade.hpp>
#include <SystemStatusFacade.hpp>
#include <pb_encode.h>
#include <pb_decode.h>
#include <SerialCommand.h>

/* This is the buffer where we will store our message. */
bool setMessageOnce = true;
LoraWanAdapter* m_LoraWanInterface;
LoraWanPriorityQueue* m_LoraWanPriorityQueue;
MeasurementFacade* m_MeasurementFacade;
SystemStatusFacade* m_SystemStatusFacade;

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
PM_Process* pmProcess = 0;
DHT_Process* dhtProcess = 0;
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
  pmProcess = new PM_Process(&Serial1, new MyPM_ProcessAdapter());
  pmProcess->init(9600);
  dhtProcess = new DHT_Process(new MyDHT_ProcessAdapter());

  m_LoraWanInterface = new LoraWanAbp();
  m_LoraWanPriorityQueue = new LoraWanPriorityQueue(m_LoraWanInterface);
  m_MeasurementFacade = new MeasurementFacade(m_LoraWanPriorityQueue);
  m_MeasurementFacade->setNewMeasurementData(0.1f,10.1f,27.0f,80.0f);
  m_SystemStatusFacade = new SystemStatusFacade(m_LoraWanPriorityQueue);
  m_SystemStatusFacade->setBatteryStatus(SystemStatusFacade::State::e_OK,18.4f);
  m_LoraWanPriorityQueue->setUpdateCycleHighPriorityPerdioc(2);
  m_LoraWanPriorityQueue->start();
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();         // process serial commands
  }
  pmProcess->pollSerialData();
  yield();                      // process Timers
  float batteryVoltage = battery->getBatteryVoltage();
 
  m_SystemStatusFacade->setBatteryStatus(SystemStatusFacade::State::e_OK,batteryVoltage);
  m_LoraWanInterface->loopOnce();
  m_LoraWanPriorityQueue->update();
}
