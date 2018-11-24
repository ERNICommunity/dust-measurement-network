#include <stdint.h>
#include <ILoraWanConfigAdapter.h>
#include <LoraWanAdapter.hpp>

LoraWanAdapter::LoraWanAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter /*= 0*/)
: m_loraWanConfigAdapter(loraWanConfigAdapter)
{ }

LoraWanAdapter::~LoraWanAdapter()
{ }

void LoraWanAdapter::setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter)
{
  m_loraWanConfigAdapter = loraWanConfigAdapter;
}

ILoraWanConfigAdapter* LoraWanAdapter::loraWanConfigAdapter()
{
  return m_loraWanConfigAdapter;
}

void LoraWanAdapter::setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{

}

bool LoraWanAdapter::isReadyToRead()
{
  return false;
}
uint64_t LoraWanAdapter::readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer)
{
  return 0;
}

uint64_t LoraWanAdapter::getSentCounterPeriodicMessage()
{
  return 0;
}

void LoraWanAdapter::loopOnce()
{

}
