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
#include <Assets.h>
#include <MyDeviceSerialNrAdapter.h>
#include <DetectorFakePersDataMemory.h>
#include <Battery.h>
#include <MyBatteryAdapter.h>
#include <PM_Process.h>
#include <MyPM_ProcessAdapter.h>
#include <DHT_Process.h>
#include <MyDHT_ProcessAdapter.h>
#include <LoraWanAbp.h>
#include <LoRaWanDriver.h>
#include <MyLoRaWanConfigAdapter.h>
#include <LoraWanPriorityQueue.h>
#include <MeasurementFacade.h>
#include <SystemStatusFacade.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <SerialCommand.h>

/* This is the buffer where we will store our message. */
bool setMessageOnce = true;
LoRaWanDriver* m_LoraWanInterface;
LoraWanPriorityQueue* m_LoraWanPriorityQueue;
MeasurementFacade* m_MeasurementFacade;
SystemStatusFacade* m_SystemStatusFacade;

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
PM_Process* pmProcess = 0;
DHT_Process* dhtProcess = 0;
Assets* assets = 0;
Battery* battery = 0;

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();

  //-----------------------------------------------------------------------------
  // Assets (inventory and persistent data)
  //-----------------------------------------------------------------------------
  assets = new Assets(new MyDeviceSerialNrAdapter(), new DetectorFakePersDataMemory());

  //-----------------------------------------------------------------------------
  // Battery Voltage Surveillance
  //-----------------------------------------------------------------------------
  BatteryThresholdConfig battCfg = { 3.6, // BATT_WARN_THRSHD [V]
                                     3.4, // BATT_STOP_THRSHD [V]
                                     3.2, // BATT_SHUT_THRSHD [V]
                                     0.1  // BATT_HYST        [V]
                                    };
  battery = new Battery(new MyBatteryAdapter(), battCfg);

  //-----------------------------------------------------------------------------
  // Sensors
  //-----------------------------------------------------------------------------
  pmProcess = new PM_Process(&Serial1, new MyPM_ProcessAdapter());
  pmProcess->init(9600);
  dhtProcess = new DHT_Process(new MyDHT_ProcessAdapter());

  //-----------------------------------------------------------------------------
  // LoRaWan
  //-----------------------------------------------------------------------------
  m_LoraWanInterface = new LoraWanAbp(new MyLoRaWanConfigAdapter(assets));

  // #TODO nid: remove this again (this is just used when working with single channel gateway)
  m_LoraWanInterface->setIsSingleChannel(true);

  m_LoraWanPriorityQueue = new LoraWanPriorityQueue(m_LoraWanInterface);
  m_MeasurementFacade = new MeasurementFacade(m_LoraWanPriorityQueue);
  m_SystemStatusFacade = new SystemStatusFacade(m_LoraWanPriorityQueue);
  m_LoraWanPriorityQueue->setUpdateCycleHighPriorityPerdioc(2);
  m_LoraWanPriorityQueue->start();
}

SystemStatusFacade::State getBatteryState()
{
  if (battery->isBattVoltageOk())
  {
    return SystemStatusFacade::State::e_OK;
  }
  if (battery->isBattVoltageBelowWarnThreshold())
  {
    return SystemStatusFacade::State::e_WARNING;
  }
  if (battery->isBattVoltageBelowStopThreshold())
  {
    return SystemStatusFacade::State::e_STOP;
  }
  if (battery->isBattVoltageBelowShutdownThreshold())
  {
    return SystemStatusFacade::State::e_SHUTDOWN;
  }
  return SystemStatusFacade::State::e_UNDEFINED;
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();         // process serial commands
  }
  pmProcess->pollSerialData();
  yield();                      // process Timers
  float pm10 = pmProcess->getPm10Average();
  float pm25 = pmProcess->getPm25Average();
  float humidity = dhtProcess->getRelHumidity();
  float temperature = dhtProcess->getTemperature();
  float batteryVoltage = battery->getBatteryVoltage();

  m_SystemStatusFacade->setBatteryStatus(getBatteryState(), batteryVoltage);
  m_MeasurementFacade->setNewMeasurementData(pm25, pm10, temperature, humidity);
  m_LoraWanInterface->loopOnce();
  m_LoraWanPriorityQueue->update();
}
