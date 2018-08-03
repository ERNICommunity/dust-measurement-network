/*
 * MyPmAdapter.cpp
 *
 *  Created on: 29.08.2017
 *      Author: nid
 */

#include <Arduino.h>
#include <MyPM_ProcessAdapter.h>

MyPM_ProcessAdapter::MyPM_ProcessAdapter()
{ }

MyPM_ProcessAdapter::~MyPM_ProcessAdapter()
{ }

void MyPM_ProcessAdapter::notifyPmChanged(float pm10, float pm25)
{
//  Serial.print("PM10: ");
//  Serial.print(pm10);
//  Serial.print(", PM2.5: ");
//  Serial.println(pm25);
}
