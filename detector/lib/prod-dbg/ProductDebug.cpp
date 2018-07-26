/*
 * ProductDebug.cpp
 *
 *  Created on: 14.06.2016
 *      Author: nid
 */

#include "ProductDebug.h"

#include <Arduino.h>
#include <Timer.h>
#include <SerialCommand.h>
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTraceContext.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <DbgPrintConsole.h>
#include <DbgTraceOut.h>
#include <AppDebug.h>


#ifdef ESP8266
extern "C"
{
  #include "user_interface.h"
}
#else
#include <RamUtils.h>
#endif

//-----------------------------------------------------------------------------

void setupProdDebugEnv()
{
  setupDebugEnv();

  Serial.println();
  Serial.println("---------------------------------------------");
  Serial.println("Hello from Wiring Controller!");
  Serial.println("---------------------------------------------");
  Serial.println();
}

