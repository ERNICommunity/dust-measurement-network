#ifndef _LoraWan_LoraWanAdapter_h_
#define _LoraWan_LoraWanAdapter_h_

#include <inttypes.h>
#include <lmic.h>
#include <hal/hal.h>

class ILoraWanConfigAdapter;
class DbgTrace_Port;
class DbgCli_Command;
class DbgCli_Topic;
class LoRaWanDbgCmd_Configure;
class LoRaWanDbgCmd_SingleChannel;

//-----------------------------------------------------------------------------

struct LmicPinMap_DraginoShield : public lmic_pinmap
{
  LmicPinMap_DraginoShield()
  : lmic_pinmap()
  {
    nss    = 10;              // chip select
    rxtx   = LMIC_UNUSED_PIN; // rx/tx control
    rst    = 9;               // reset
    dio[0] = 2;               // DIO0
    dio[1] = 6;               // DIO1
    dio[2] = 7;               // DIO2
  }
};

//-----------------------------------------------------------------------------

struct LmicPinMap_AdafruitFeatherM0 : public lmic_pinmap
{
  LmicPinMap_AdafruitFeatherM0()
  : lmic_pinmap()
  {
    nss    = 8;                // chip select
    rxtx   = LMIC_UNUSED_PIN;  // rx/tx control
    rst    = 4;                // reset
    dio[0] = 6;                // DIO0   (=> needs external jumoper!)
    dio[1] = 3;                // DIO1
    dio[2] = LMIC_UNUSED_PIN;  // DIO2
  }
};


//-----------------------------------------------------------------------------

struct LmicPinMap_AdafruitFeather32u4 : public lmic_pinmap
{
  LmicPinMap_AdafruitFeather32u4()
  : lmic_pinmap()
  {
    nss    = 8;                // chip select
    rxtx   = LMIC_UNUSED_PIN;  // rx/tx control
    rst    = 4;                // reset
    dio[0] = 7;                // DIO0
    dio[1] = 6;                // DIO1
    dio[2] = LMIC_UNUSED_PIN;  // DIO2
  }
};

//-----------------------------------------------------------------------------

class LoRaWanDriver
{
  public:
    typedef enum
    {
      LLPMS_AdafruitFeatherM0,
      LLPMS_AdafruitFeather32u4,
      LLPMS_DraginoLoRaShield,
      LLPMS_Unknown
    } LwdLmicPinMapSelection;

  LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter = 0, LwdLmicPinMapSelection lmicPinMapSelection = LLPMS_AdafruitFeatherM0);
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

  public:
//    static lmic_pinmap* s_lmicPinmap;

  private: // forbidden default functions
    LoRaWanDriver& operator = (const LoRaWanDriver& src); // assignment operator
    LoRaWanDriver(const LoRaWanDriver& src);              // copy constructor
};
#endif
