#include <stdint.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
//#include <DbgCliCommand.h>
#include <DbgCliTopic.h>
#include <ILoraWanConfigAdapter.h>
#include <LoRaWanDbgCmd.h>
#include <LoRaWanDriver.hpp>

LoRaWanDriver* LoRaWanDriver::s_loRaWanDriver = 0;

LoRaWanDriver::LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter /*= 0*/)
: m_loraWanConfigAdapter(loraWanConfigAdapter)
, m_trPort(new DbgTrace_Port("lora", DbgTrace_Level::notice))
, m_dbgCliTopic(new DbgCli_Topic(DbgCli_Node::RootNode(), "lora", "LoRaWan Driver."))
, m_dbgCliLoRaCfg(new LoRaWanDbgCmd_Configure(this))
, m_dbgCliSingleChannel(new LoRaWanDbgCmd_SingleChannel(this))
, m_isSingleChannel(false)
{
  s_loRaWanDriver = this;
}

LoRaWanDriver::~LoRaWanDriver()
{ }

void LoRaWanDriver::setIsSingleChannel(bool isSingleChannel /*= true*/)
{
  m_isSingleChannel = isSingleChannel;
}

bool LoRaWanDriver::getIsSinglechannel()
{
  return m_isSingleChannel;
}

void LoRaWanDriver::setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter)
{
  m_loraWanConfigAdapter = loraWanConfigAdapter;
}

ILoraWanConfigAdapter* LoRaWanDriver::loraWanConfigAdapter()
{
  return m_loraWanConfigAdapter;
}

DbgTrace_Port* LoRaWanDriver::trPort()
{
  return m_trPort;
}


void LoRaWanDriver::setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{

}

bool LoRaWanDriver::isReadyToRead()
{
  return false;
}
uint64_t LoRaWanDriver::readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer)
{
  return 0;
}

uint64_t LoRaWanDriver::getSentCounterPeriodicMessage()
{
  return 0;
}

void LoRaWanDriver::loopOnce()
{

}

LoRaWanDriver* LoRaWanDriver::getLoRaWanDriver()
{
  return s_loRaWanDriver;
}
