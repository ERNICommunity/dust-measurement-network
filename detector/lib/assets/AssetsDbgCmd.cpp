/*
 * AssetsDbgCmd.cpp
 *
 *  Created on: 09.11.2018
 *      Author: nid
 */

#include <stdlib.h>
#include <Arduino.h>
#include <Assets.h>
#include <DbgCliTopic.h>
#include <AssetsDbgCmd.h>
#include <IAssetsDeviceSerialNrAdapter.h>
#include <DetectorFakePersDataMemory.h>

AssetsDbgCmd_SetDeviceSerial::AssetsDbgCmd_SetDeviceSerial(Assets* assets)
: DbgCli_Command(assets->getCliTopicAssetsDeviceSerial(), "set", "Set Device Serial Number.")
, m_assets(assets)
{ }

AssetsDbgCmd_SetDeviceSerial::~AssetsDbgCmd_SetDeviceSerial()
{ }

void AssetsDbgCmd_SetDeviceSerial::printUsage()
{
  Serial.println("dbg assets dvcser set - usage: <DeviceSerialNr>");
}

void AssetsDbgCmd_SetDeviceSerial::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle != 1)
  {
    printUsage();
  }
  else
  {
    if (0 != m_assets)
    {
      m_assets->setDeviceSerialNr(atoi(args[idxToFirstArgToHandle]));
    }
  }
}

//-----------------------------------------------------------------------------

AssetsDbgCmd_GetDeviceSerial::AssetsDbgCmd_GetDeviceSerial(Assets* assets)
: DbgCli_Command(assets->getCliTopicAssetsDeviceSerial(), "get", "Get Device Serial Number.")
, m_assets(assets)
{ }

AssetsDbgCmd_GetDeviceSerial::~AssetsDbgCmd_GetDeviceSerial()
{ }

void AssetsDbgCmd_GetDeviceSerial::printUsage()
{
  Serial.println("dbg assets dvcser get - usage: - ");
}

void AssetsDbgCmd_GetDeviceSerial::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle != 0)
  {
    printUsage();
  }
  else
  {
    if (0 != m_assets)
    {
      Serial.println("Assets - LoRa Keys:");
      if (0 != m_assets->getDeviceSerialNrAdapter())
      {
        Serial.print("- Provisioned Data: ");
        Serial.println(m_assets->getDeviceSerialNrAdapter()->getDeviceSerialNr());
      }
      else
      Serial.print("- Internal Data: ");
      Serial.println(m_assets->getDeviceSerialNr());
    }
  }
}

//-----------------------------------------------------------------------------

AssetsDbgCmd_GetLoRaKeys::AssetsDbgCmd_GetLoRaKeys(Assets* assets)
: DbgCli_Command(assets->getCliTopicAssetsDeviceSerial(), "get", "Get LoRa Keys.")
, m_assets(assets)
{ }

AssetsDbgCmd_GetLoRaKeys::~AssetsDbgCmd_GetLoRaKeys()
{ }

void AssetsDbgCmd_GetLoRaKeys::printUsage()
{
  Serial.println("dbg assets lorakeys get - usage: - ");
}

void AssetsDbgCmd_GetLoRaKeys::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle != 0)
  {
    printUsage();
  }
  else
  {
    if (0 != m_assets)
    {
      Serial.println("Assets - Device Serial Number:");
      if (0 != m_assets->getDeviceSerialNrAdapter())
      {
        char printBuffer[DetectorFakePersDataMemory::s_numMaxChars+1];

        m_assets->getDeviceId(printBuffer, sizeof(printBuffer));
        Serial.print("- Device Id:               ");
        Serial.println(printBuffer);

        m_assets->getDevAddr(printBuffer, sizeof(printBuffer));
        Serial.print("- Device Addr:             ");
        Serial.println();

        m_assets->getNwkSKey(printBuffer, sizeof(printBuffer));
        Serial.print("- Network Session Key:     ");
        Serial.println(printBuffer);

        m_assets->getAppSKey(printBuffer, sizeof(printBuffer));
        Serial.print("- Application Session Key: ");
        Serial.println();
      }
      else
      {
        Serial.println("Persistent Data Memory not accessible.");
      }
    }
  }
}
