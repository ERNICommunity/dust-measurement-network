#include <stdint.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
#include <DbgCliTopic.h>
#include <ILoraWanConfigAdapter.h>
#include <ILoraWanRxDataEventAdapter.h>
#include <LoRaWanDbgCmd.h>
#include <LoRaWanDriver.h>

LoRaWanDriver* LoRaWanDriver::s_loRaWanDriver = 0;
const unsigned int LoRaWanDriver::s_defaultTxInterval = 15;

LoRaWanDriver::LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter /*= 0*/,
                             ILoraWanRxDataEventAdapter* loraWanRxDataEventAdapter /*= 0*/,
                             unsigned int txInterval /*= s_defaultTxInterval*/)
: m_loraWanConfigAdapter(loraWanConfigAdapter)
, m_loraWanRxDataEventAdapter(loraWanRxDataEventAdapter)
, m_trPort(new DbgTrace_Port("lora", DbgTrace_Level::notice))
, m_dbgCliTopic(new DbgCli_Topic(DbgCli_Node::RootNode(), "lora", "LoRaWan Driver."))
, m_dbgCliLoRaCfg(new LoRaWanDbgCmd_Configure(this))
, m_dbgCliSingleChannel(new LoRaWanDbgCmd_SingleChannel(this))
, m_isSingleChannel(false)
, m_txInterval(txInterval)
{
  s_loRaWanDriver = this;
}

LoRaWanDriver::~LoRaWanDriver()
{
  s_loRaWanDriver = 0;
}

void LoRaWanDriver::setIsSingleChannel(bool isSingleChannel /*= true*/)
{
  m_isSingleChannel = isSingleChannel;
}

bool LoRaWanDriver::getIsSinglechannel()
{
  return m_isSingleChannel;
}

unsigned int LoRaWanDriver::getTxInterval()
{
  return m_txInterval;
}

void LoRaWanDriver::setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter)
{
  m_loraWanConfigAdapter = loraWanConfigAdapter;
}

ILoraWanConfigAdapter* LoRaWanDriver::loraWanConfigAdapter()
{
  return m_loraWanConfigAdapter;
}

void LoRaWanDriver::setLoraWanRxDataEventAdapter(ILoraWanRxDataEventAdapter* loraWanRxDataEventAdapter)
{
  m_loraWanRxDataEventAdapter = loraWanRxDataEventAdapter;
}

ILoraWanRxDataEventAdapter* LoRaWanDriver::loraWanRxDataEventAdapter()
{
  return m_loraWanRxDataEventAdapter;
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
