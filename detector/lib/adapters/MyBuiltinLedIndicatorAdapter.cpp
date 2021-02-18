/*
 * MyBuiltinLedIndicatorAdapter.cpp
 *
 *  Created on: 04.11.2019
 *      Author: nid
 */

#include <Arduino.h>
#include "MyBuiltinLedIndicatorAdapter.h"

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif


MyBuiltinLedIndicatorAdapter::MyBuiltinLedIndicatorAdapter()
{
  // initialize built in LED pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // switch LED off
  setLed(false);
}

MyBuiltinLedIndicatorAdapter::~MyBuiltinLedIndicatorAdapter()
{ }

void MyBuiltinLedIndicatorAdapter::notifyStatusChange(bool status)
{
  setLed(status);
}

void MyBuiltinLedIndicatorAdapter::setLed(bool isOn)
{
#ifdef ESP8266
  // the built-in LED logic on ESP8266 module is inverted!
  isOn = !isOn;
#endif
  digitalWrite(LED_BUILTIN, isOn);
}
