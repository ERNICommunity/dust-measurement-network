#include <stdint.h>
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>
//#include <DbgCliCommand.h>
#include <DbgCliTopic.h>
#include <ILoraWanConfigAdapter.h>
#include <LoRaWanDbgCmd.h>
#include <LoRaWanDriver.h>

LoRaWanDriver* LoRaWanDriver::s_loRaWanDriver = 0;
//extern const lmic_pinmap lmic_pins;
//lmic_pinmap* LoRaWanDriver::s_lmicPinmap = *(const_cast<lmic_pinmap>(lmic_pins));

LoRaWanDriver::LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter /*= 0*/,
    LoRaWanDriver::LwdLmicPinMapSelection lmicPinMapSelection /* = LLPMS_AdafruitFeatherM0*/)
: m_loraWanConfigAdapter(loraWanConfigAdapter)
, m_trPort(new DbgTrace_Port("lora", DbgTrace_Level::notice))
, m_dbgCliTopic(new DbgCli_Topic(DbgCli_Node::RootNode(), "lora", "LoRaWan Driver."))
, m_dbgCliLoRaCfg(new LoRaWanDbgCmd_Configure(this))
, m_dbgCliSingleChannel(new LoRaWanDbgCmd_SingleChannel(this))
, m_isSingleChannel(false)
{
//  switch (lmicPinMapSelection)
//  {
//    case LLPMS_AdafruitFeather32u4:
//      s_lmicPinmap = new LmicPinMap_AdafruitFeather32u4();
//      break;
//    case LLPMS_DraginoLoRaShield:
//      s_lmicPinmap = new LmicPinMap_DraginoShield();
//      break;
//
//    case LLPMS_AdafruitFeatherM0:
//    default:
//      s_lmicPinmap = new LmicPinMap_AdafruitFeatherM0();
//      break;
//  }
//  (const_cast<lmic_pinmap>(lmic_pins)).nss   = s_lmicPinmap->nss;
//  lmic_pins.rxtx  = s_lmicPinmap->rxtx;
//  lmic_pins.rst   = s_lmicPinmap->rst;
//  os_init_ex(s_lmicPinmap);
  s_loRaWanDriver = this;
}

LoRaWanDriver::~LoRaWanDriver()
{
  s_loRaWanDriver = 0;

//  delete s_lmicPinmap;
//  s_lmicPinmap = 0;
}

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
