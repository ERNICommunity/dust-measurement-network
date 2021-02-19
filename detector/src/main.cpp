/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>    // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <SpinTimer.h>        // pio lib install 11599, lib details see https://github.com/dniklaus/spin-timer

// private libraries
#include <ProductDebug.h>
#include <Assets.h>
#include <MyDeviceSerialNrAdapter.h>
#include <DetectorFakePersDataMemory.h>
#include <Battery.h>
#include <MyBatteryAdapter.h>
#include <PM_Process.h>
#include <MyPM_ProcessAdapter.h>
#include <DHT_Process.h>
#include <MyDHT_ProcessAdapter.h>
#include <LoraWanPriorityQueue.h>
#include <MeasurementFacade.h>
#include <SystemStatusFacade.h>
#include <LoraWanAbp.h>
#include <LoRaWanDriver.h>
#include <MyLoRaWanConfigAdapter.h>
#include <Indicator.h>
#include <MyBuiltinLedIndicatorAdapter.h>
#include <LoRaWanRxDataToStatusLedAdapter.h>
//#include <MyLoRaWanTxDataEventAdapter.h>   // TODO: implementation not used up to now!
#include <MyMeasuremenFacadeAdapter.h>
#include <MySystemStatusFacadeAdapter.h>

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

SerialCommand*  sCmd = 0;
PM_Process*     pmProcess = 0;
DHT_Process*    dhtProcess = 0;
Assets*         assets = 0;
Battery*        battery = 0;
Indicator*      statusLed  = 0;   // indicator implementation for built in LED

void setup()
{
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

  //---------------------------------------------------------------------------
  // Status LED (Indicator)
  //---------------------------------------------------------------------------
  statusLed = new Indicator("led", "Built in LED.");
  statusLed->assignAdapter(new MyBuiltinLedIndicatorAdapter());

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

  // #TODO nid: remove this again (this is just used when working with single channel gateway)
  //  loRaWanInterface->setIsSingleChannel(true);

  loRaWanPriorityQueue = new LoraWanPriorityQueue(loRaWanInterface);

  measurementFacade = new MeasurementFacade(loRaWanPriorityQueue);
  measurementFacade->attachAdapter(new MyMeasuremenFacadeAdapter(measurementFacade, pmProcess, dhtProcess));

  systemStatusFacade = new SystemStatusFacade(loRaWanPriorityQueue);
  systemStatusFacade->attachAdapter(new MySystemStatusFacadeAdapter(battery, systemStatusFacade));

  loRaWanPriorityQueue->setUpdateCycleHighPriorityPerdioc(2);
  loRaWanPriorityQueue->start();

  loRaWanInterface->setLoraWanRxDataEventAdapter(new LoRaWanRxDataToStatusLedAdapter(statusLed, loRaWanInterface));
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();         // process serial commands
  }

  pmProcess->pollSerialData();

  systemStatusFacade->updateSystemStatus();
  measurementFacade->updateMeasurementData();

  loRaWanPriorityQueue->update();

  scheduleTimers();             // process Timers

  loRaWanInterface->loopOnce();
}
