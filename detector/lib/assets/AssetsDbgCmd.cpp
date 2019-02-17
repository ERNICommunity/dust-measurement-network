/*
 * AssetsDbgCmd.cpp
 *
 *  Created on: 09.11.2018
 *      Author: nid
 */

#include <stdlib.h>
#include <Assets.h>
#include <AssetsDbgCmd.h>
#include <DbgCliTopic.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
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
  TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "dbg assets dvcser set - usage: <DeviceSerialNr>");
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
  TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "dbg assets dvcser get - usage: - ");
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
      TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "Assets - Device Serial Number:");
      if (0 != m_assets->getDeviceSerialNrAdapter())
      {
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Provisioned Data (by Production): %d", m_assets->getDeviceSerialNrAdapter()->getDeviceSerialNr());
      }
      TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Current Data: %d", m_assets->getDeviceSerialNr());
    }
  }
}

//-----------------------------------------------------------------------------

AssetsDbgCmd_GetLoRaKeys::AssetsDbgCmd_GetLoRaKeys(Assets* assets)
: DbgCli_Command(assets->getCliTopicAssetsLoRaKeys(), "get", "Get LoRa Keys.")
, m_assets(assets)
{ }

AssetsDbgCmd_GetLoRaKeys::~AssetsDbgCmd_GetLoRaKeys()
{ }

void AssetsDbgCmd_GetLoRaKeys::printUsage()
{
  TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "dbg assets lorakeys get - usage: - ");
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
      TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "Assets - LoRa Keys:");
      if (0 != m_assets->getPersistentDataMemory())
      {
        char printBuffer[DetectorFakePersDataMemory::s_numMaxChars+1];
        m_assets->getDeviceId(printBuffer, sizeof(printBuffer));
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Device Id:               %s", printBuffer);

        m_assets->getDevAddr(printBuffer, sizeof(printBuffer));
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Device Addr:             %s", printBuffer);

        m_assets->getNwkSKey(printBuffer, sizeof(printBuffer));
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Network Session Key:     %s", printBuffer);

        m_assets->getAppSKey(printBuffer, sizeof(printBuffer));
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "- Application Session Key: %s", printBuffer);
      }
      else
      {
        TR_PRINTF(m_assets->trPort(), DbgTrace_Level::alert, "Persistent Data Memory not accessible.");
      }
    }
  }
}
