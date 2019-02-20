/*
 * LoRaWanDbgCmd.cpp
 *
 *  Created on: 12.01.2019
 *      Author: nid
 */
#include <LoRaWanDbgCmd.h>
#include <DbgCliTopic.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <LoRaWanDriver.h>

LoRaWanDbgCmd_Configure::LoRaWanDbgCmd_Configure(LoRaWanDriver* loRaWanDriver)
: DbgCli_Command(loRaWanDriver->getCliTopic(), "cfg", "Configure the LoRaWan Driver.")
, m_loRaWanDriver(loRaWanDriver)
{ }

LoRaWanDbgCmd_Configure::~LoRaWanDbgCmd_Configure()
{ }

void LoRaWanDbgCmd_Configure::printUsage()
{
  TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "Usage: dbg lora cfg\n");
}

void LoRaWanDbgCmd_Configure::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle != 0)
  {
    TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "%s\n", getHelpText());
    printUsage();
  }
  else
  {
    if (0 != m_loRaWanDriver)
    {
      TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "Applying configuration to LoRaWan driver.");
      m_loRaWanDriver->configure(true);
    }
  }
}

//-----------------------------------------------------------------------------

LoRaWanDbgCmd_SingleChannel::LoRaWanDbgCmd_SingleChannel(LoRaWanDriver* loRaWanDriver)
: DbgCli_Command(loRaWanDriver->getCliTopic(), "sch", "Get / set the LoRaWan Driver's single channel mode config status.")
, m_loRaWanDriver(loRaWanDriver)
{ }

LoRaWanDbgCmd_SingleChannel::~LoRaWanDbgCmd_SingleChannel()
{ }

void LoRaWanDbgCmd_SingleChannel::printUsage()
{
  TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "Usage: dbg lora sch - get the single channel mode\n dbg lora sch <isSingleChannel> {1|0} - set the single channel mode (1: active, 0: inactive)");
}

void LoRaWanDbgCmd_SingleChannel::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (0 != m_loRaWanDriver)
  {
    if (argc - idxToFirstArgToHandle == 0)
    {
      TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "Single channel mode is %s", m_loRaWanDriver->getIsSinglechannel() ? "active" : "inactive");
    }
    else if (argc - idxToFirstArgToHandle == 1)
    {
      bool isSingleChannel = atoi(args[idxToFirstArgToHandle]) > 0;
      TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "Setting single channel mode %s", isSingleChannel ? "active" : "inactive");
      if (m_loRaWanDriver->getIsSinglechannel() != isSingleChannel)
      {
        TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "The single channel mode has changed, applying configuration to LoRaWan driver.");
        m_loRaWanDriver->setIsSingleChannel(isSingleChannel);
        m_loRaWanDriver->configure(true);
      }
    }
    else
    {
      {
        TR_PRINTF(m_loRaWanDriver->trPort(), DbgTrace_Level::alert, "%s\n", getHelpText());
      }
      printUsage();
    }
  }
}

//-----------------------------------------------------------------------------

