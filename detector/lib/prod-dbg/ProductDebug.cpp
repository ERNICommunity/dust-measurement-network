/*
 * ProductDebug.cpp
 *
 *  Created on: 14.06.2016
 *      Author: nid
 */

#include "ProductDebug.h"

#include <Arduino.h>
#include <Timer.h>
#include <Assets.h>
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
// Asset Commands
//-----------------------------------------------------------------------------
class DbgCli_Cmd_AssetReadId : public DbgCli_Command
{
private:
  Assets* m_assets;
public:
  DbgCli_Cmd_AssetIdRead(DbgCli_Topic* assetIdTopic, Assets* assets)
  : DbgCli_Command(assetTopic, "rd", "Assets - Read Device Id.")
  , m_assets(assets)
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    Serial.println();
    if (argc - idxToFirstArgToHandle > 0)
    {
      printUsage();
    }
    else
    {
      Serial.println();
      Serial.print("Assets - Device Id: ");
      if (0 != m_assets)
      {
        Serial.println(m_assets->getDeviceId());
      }
      else
      {
        Serial.println("NONE");
      }
      Serial.println();
    }
    Serial.println();
  }
};

//-----------------------------------------------------------------------------

void setupProdDebugEnv()
{
  setupDebugEnv();

  Serial.println();
  Serial.println("-----------------------------------------------------------------");
  Serial.println("Hello from Dust Measurement Network Detector Firmware Controller!");
  Serial.println("-----------------------------------------------------------------");
  Serial.println();
}

