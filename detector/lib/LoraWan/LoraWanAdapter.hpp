#ifndef _LoraWan_LoraWanAdapter_h_
#define _LoraWan_LoraWanAdapter_h_
#include <inttypes.h>

class ILoraWanConfigAdapter;
class DbgTrace_Port;

class LoraWanAdapter
{
  public:
    LoraWanAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter = 0);
    virtual ~LoraWanAdapter();
    virtual void setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    virtual uint64_t getSentCounterPeriodicMessage();
    virtual bool isReadyToRead();
    virtual uint64_t readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer);
    virtual void loopOnce();
    void setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter);
    ILoraWanConfigAdapter* loraWanConfigAdapter();

//  protected:
    virtual void configure() = 0;

    DbgTrace_Port* trPort();

  private:
    ILoraWanConfigAdapter* m_loraWanConfigAdapter;
    DbgTrace_Port* m_trPort;

  private: // forbidden default functions
    LoraWanAdapter& operator = (const LoraWanAdapter& src); // assignment operator
    LoraWanAdapter(const LoraWanAdapter& src);              // copy constructor
};
#endif
