/*
 * MyDeviceSerialNrAdapter.cpp
 *
 *  Created on: 06.11.2018
 *      Author: nid
 */

#include <Arduino.h>
#include <FramDeviceSerialNrAdapter.h>
#include <Adafruit_FRAM_I2C.h>

FramDeviceSerialNrAdapter::FramDeviceSerialNrAdapter()
{}

FramDeviceSerialNrAdapter::~FramDeviceSerialNrAdapter()
{}

byte addrTestData = 0x50;
byte testData = 0x04;

unsigned long int FramDeviceSerialNrAdapter::getDeviceSerialNr()
{
  unsigned long int deviceSerialNr = 0;

  Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
  fram.begin();
  fram.write8(addrTestData, testData);
  deviceSerialNr = fram.read8(addrTestData);

  return deviceSerialNr;
}
