/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

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
#include <pb_encode.h>
#include <pb_decode.h>
#include <LoraWanPriorityQueue.h>
#include <MeasurementFacade.h>
#include <SystemStatusFacade.h>
#include <LoraWanAbp.h>
#include <LoRaWanDriver.h>
#include <MyLoRaWanConfigAdapter.h>

LoRaWanDriver* loRaWanInterface = 0;

// Pin mapping
//#if defined(ARDUINO_SAMD_FEATHER_M0)
const lmic_pinmap lmic_pins = LmicPinMap_AdafruitFeatherM0();
//#elif defined (__arm__) && defined (__SAM3X8E__)              // Arduino Due => Dragino Shield
//const lmic_pinmap lmic_pins = LmicPinMap_DraginoShield();
//#elif defined (__avr__)                                       // Arduino Uno or Mega 2560 => Dragino Shield
//const lmic_pinmap lmic_pins = LmicPinMap_DraginoShield();
//#endif

LoraWanPriorityQueue* loRaWanPriorityQueue = 0;
MeasurementFacade* measurementFacade = 0;
SystemStatusFacade* systemStatusFacade = 0;

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

  delay(5000);

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
  loRaWanInterface = new LoraWanAbp(new MyLoRaWanConfigAdapter(assets));
  loRaWanInterface->configure(true);

  // #TODO nid: remove this again (this is just used when working with single channel gateway)
//  m_LoraWanInterface->setIsSingleChannel(true);

  loRaWanPriorityQueue = new LoraWanPriorityQueue(loRaWanInterface);
  measurementFacade = new MeasurementFacade(loRaWanPriorityQueue);
  systemStatusFacade = new SystemStatusFacade(loRaWanPriorityQueue);
  loRaWanPriorityQueue->setUpdateCycleHighPriorityPerdioc(2);
  loRaWanPriorityQueue->start();
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

  systemStatusFacade->setBatteryStatus(getBatteryState(), batteryVoltage);
  measurementFacade->setNewMeasurementData(pm25, pm10, temperature, humidity);
  loRaWanInterface->loopOnce();
  loRaWanPriorityQueue->update();
}
