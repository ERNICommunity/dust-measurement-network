/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>
#include <Wire.h>

// PlatformIO libraries
#include <SerialCommand.h>    // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <Timer.h>            // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer
#include <Adafruit_Sensor.h>  // pio lib 19, 31, lib details see https://github.com/adafruit/DHT-sensor-library
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_FRAM_I2C.h> // pio lib 658, lib details see https://github.com/adafruit/Adafruit_FRAM_I2C

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
#include <ToggleButton.h>
#include <LoRaWanRxDataToStatusLedAdapter.h>
#include <MyLoRaWanTxDataEventAdapter.h>
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

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
PM_Process* pmProcess = 0;
DHT_Process* dhtProcess = 0;
Assets* assets = 0;
Battery* battery = 0;
ToggleButton* statusLed = 0;

Adafruit_FRAM_I2C* fram = 0;

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  delay(5000);

  setupProdDebugEnv();

  //-----------------------------------------------------------------------------
  // FRAM Test
  //-----------------------------------------------------------------------------
  fram = new Adafruit_FRAM_I2C();
  if (0 != fram)
  {
    if (fram->begin())
    {  // you can stick the new i2c addr in here, e.g. begin(0x51);
      Serial.println("Found I2C FRAM");
    }
    else
    {
      Serial.println("No I2C FRAM found ... check your connections\r\n");
      fram = 0;
    }
  }
  if (0 != fram)
  {
    // Read the first byte
    uint8_t test = fram->read8(0x0);
    Serial.print("Restarted ");
    Serial.print(test);
    Serial.println(" times");
    // Test write ++
    fram->write8(0x0, test + 1);

    // dump the entire 32K of memory!
    uint8_t value;
    for (uint16_t a = 0; a < 32768; a++)
    {
      value = fram->read8(a);
      if ((a % 32) == 0)
      {
        Serial.print("\n 0x");
        Serial.print(a, HEX);
        Serial.print(": ");
      }
      Serial.print("0x");
      if (value < 0x1)
        Serial.print('0');
      Serial.print(value, HEX);
      Serial.print(" ");
    }
  }

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
  // Status LED (ToggleButton)
  //---------------------------------------------------------------------------
  statusLed = new ToggleButton(ToggleButton::BTN_NC, BUILTIN_LED);

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

  yield();                      // process Timers

  loRaWanInterface->loopOnce();
}
