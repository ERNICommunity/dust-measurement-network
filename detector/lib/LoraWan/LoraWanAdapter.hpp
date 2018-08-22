#ifndef _LoraWan_LoraWanAdapter_h_
#define _LoraWan_LoraWanAdapter_h_
#include <inttypes.h>

class LoraWanAdapter
{
  public:
    LoraWanAdapter();
    virtual void setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    virtual uint64_t getSentCounterPeriodicMessage();
    virtual bool isReadyToRead();
    virtual uint64_t readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer);
    virtual void loopOnce();
};
#endif