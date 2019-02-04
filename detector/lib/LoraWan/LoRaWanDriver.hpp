#ifndef _LoraWan_LoraWanAdapter_h_
#define _LoraWan_LoraWanAdapter_h_
#include <inttypes.h>

class ILoraWanConfigAdapter;
class DbgTrace_Port;
class DbgCli_Command;
class DbgCli_Topic;
class LoRaWanDbgCmd_Configure;
class LoRaWanDbgCmd_SingleChannel;


class LoRaWanDriver
{
  public:
    LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter = 0);
    virtual ~LoRaWanDriver();
    virtual void setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
    virtual uint64_t getSentCounterPeriodicMessage();
    virtual bool isReadyToRead();
    virtual uint64_t readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer);
    virtual void loopOnce();
    void setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter);
    ILoraWanConfigAdapter* loraWanConfigAdapter();

    virtual void configure(bool isForced = false) = 0;

    void setIsSingleChannel(bool isSingleChannel = true);
    bool getIsSinglechannel();

    DbgCli_Topic* getCliTopic() { return m_dbgCliTopic; }
    DbgTrace_Port* trPort();
    static LoRaWanDriver* getLoRaWanDriver();

  private:
    ILoraWanConfigAdapter* m_loraWanConfigAdapter;
    DbgTrace_Port* m_trPort;
    DbgCli_Topic* m_dbgCliTopic;
    LoRaWanDbgCmd_Configure* m_dbgCliLoRaCfg;
    LoRaWanDbgCmd_SingleChannel* m_dbgCliSingleChannel;
    bool m_isSingleChannel;
    static LoRaWanDriver* s_loRaWanDriver;

  private: // forbidden default functions
    LoRaWanDriver& operator = (const LoRaWanDriver& src); // assignment operator
    LoRaWanDriver(const LoRaWanDriver& src);              // copy constructor
};
#endif
