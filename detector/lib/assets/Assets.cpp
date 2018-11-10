 /*
  * Assets.h
  *
  *  Created on: 25.08.2018
  *      Author: kath
  */

#include<string.h>
//#include "Adafruit_FRAM_I2C.h" // PIO Lib: 658 -> https://platformio.org/lib/show/658/Adafruit%20FRAM%20I2C
#include <Assets.h>
#include <IAssetsDeviceSerialNrAdapter.h>
#include <DbgCliTopic.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <AssetsDbgCmd.h>

Assets::Assets(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter /* = 0*/)
: m_trPort(new DbgTrace_Port("assets", DbgTrace_Level::info))
, m_dbgCliTopicAssets(new DbgCli_Topic(DbgCli_Node::RootNode(), "assets", "Assets."))
, m_dbgCliTopicAssetsDeviceSerial(new DbgCli_Topic(m_dbgCliTopicAssets, "dvcser", "Device Serial Number."))
, m_dbgCliSetDeviceSerialCmd(new AssetsDbgCmd_SetDeviceSerial(this))
, m_dbgCliGetDeviceSerialCmd(new AssetsDbgCmd_GetDeviceSerial(this))
, m_deviceSerialNr(0)
, m_deviceSerialNrAdapter(0)
{
  setDeviceSerialNrAdapter(deviceSerialNrAdapter);
}

Assets::~Assets()
{ }

void Assets::setDeviceSerialNrAdapter(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter)
{
  m_deviceSerialNrAdapter = deviceSerialNrAdapter;
  if (0 != m_deviceSerialNrAdapter)
  {
    m_deviceSerialNr = m_deviceSerialNrAdapter->getDeviceSerialNr();
  }
  else
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::error, "ERROR IN Assets: no deviceSerialNrAdapter set! Assume deviceSerialNr = 0");
    m_deviceSerialNr = 0;
  }
  TR_PRINTF(m_trPort, DbgTrace_Level::info, "Assets: deviceSerialNr = %u", m_deviceSerialNr);
}

IAssetsDeviceSerialNrAdapter* Assets::getDeviceSerialNrAdapter()
{
  return m_deviceSerialNrAdapter;
}

unsigned long int Assets::getDeviceSerialNr()
{
  return m_deviceSerialNr;
}

void Assets::setDeviceSerialNr(unsigned long int deviceSerialNr)
{
  m_deviceSerialNr = deviceSerialNr;
}
