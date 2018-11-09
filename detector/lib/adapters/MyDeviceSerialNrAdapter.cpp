/*
 * MyDeviceSerialNrAdapter.cpp
 *
 *  Created on: 06.11.2018
 *      Author: nid
 */

#include <Arduino.h>
#include <MyDeviceSerialNrAdapter.h>


const int MyDeviceSerialNrAdapter::s_serialNr_bit0 = A0;
const int MyDeviceSerialNrAdapter::s_serialNr_bit1 = A1;
const int MyDeviceSerialNrAdapter::s_serialNr_bit2 = A2;
const int MyDeviceSerialNrAdapter::s_serialNr_bit3 = A3;
const int MyDeviceSerialNrAdapter::s_serialNr_bit4 = A4;

MyDeviceSerialNrAdapter::MyDeviceSerialNrAdapter()
{
  pinMode(s_serialNr_bit0, INPUT_PULLUP);
  pinMode(s_serialNr_bit1, INPUT_PULLUP);
  pinMode(s_serialNr_bit2, INPUT_PULLUP);
  pinMode(s_serialNr_bit3, INPUT_PULLUP);
  pinMode(s_serialNr_bit4, INPUT_PULLUP);
}

MyDeviceSerialNrAdapter::~MyDeviceSerialNrAdapter()
{ }

unsigned long int MyDeviceSerialNrAdapter::getDeviceSerialNr()
{
  unsigned long int deviceSerialNr = 0;
  deviceSerialNr = digitalRead(s_serialNr_bit0) +
                  (digitalRead(s_serialNr_bit1) << 1) +
                  (digitalRead(s_serialNr_bit2) << 2) +
                  (digitalRead(s_serialNr_bit3) << 3) +
                  (digitalRead(s_serialNr_bit4) << 4) ;
  return deviceSerialNr;
}
