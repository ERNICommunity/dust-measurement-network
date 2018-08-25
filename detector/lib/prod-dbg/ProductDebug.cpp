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
extern Assets* assets;


class DbgCli_Cmd_AssetId : public DbgCli_Command
{
private:
  Assets* m_assets;
public:
  DbgCli_Cmd_AssetId(DbgCli_Topic* assetTopic, Assets* assets)
  : DbgCli_Command(assetTopic, "id", "Assets - read/write device Id.")
  , m_assets(assets)
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    Serial.println();
    if (argc - idxToFirstArgToHandle == 0)
    {
      Serial.print("Assets - Read Device Id: ");
      if (0 == m_assets)
      {
        Serial.println("ERROR! No assets object available!");
      }
      else
      {
        Serial.println(m_assets->getDeviceId());
      }
    }
    else if (argc - idxToFirstArgToHandle == 1)
    {
      // write the given value
      Serial.print("Assets - Writing Device Id: ");
      Serial.print(args[idxToFirstArgToHandle]);
      if (0 == m_assets)
      {
        Serial.println("ERROR! No assets object available!");
      }
      else
      {
        m_assets->setDeviceId(args[idxToFirstArgToHandle]);
      }
    }
    else
    {
      printUsage();
    }
    Serial.println();
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg asst id [<deviceId-to-write>]");
  }
};

//-----------------------------------------------------------------------------

void setupProdDebugEnv()
{
  setupDebugEnv();

  //-----------------------------------------------------------------------------
  // Asset Commands
  //-----------------------------------------------------------------------------
  DbgCli_Topic* assetTopic = new DbgCli_Topic(DbgCli_Node::RootNode(), "asst", "Assets debug commands");
  new DbgCli_Cmd_AssetId(assetTopic, assets);

  Serial.println();
  Serial.println("-----------------------------------------------------------------");
  Serial.println("Hello from Dust Measurement Network Detector Firmware Controller!");
  Serial.println("-----------------------------------------------------------------");
  Serial.println();
}

