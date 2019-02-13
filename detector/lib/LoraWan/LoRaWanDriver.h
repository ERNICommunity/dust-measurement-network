#ifndef _LoraWan_LoraWanAdapter_h_
#define _LoraWan_LoraWanAdapter_h_

#include <inttypes.h>
#include <lmic.h>
#include <hal/hal.h>

class ILoraWanConfigAdapter;
class ILoraWanRxDataEventAdapter;
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
  LoRaWanDriver(ILoraWanConfigAdapter* loraWanConfigAdapter = 0,
                ILoraWanRxDataEventAdapter* loraWanRxDataEventAdapter = 0,
                unsigned int txInterval = s_defaultTxInterval);
  virtual ~LoRaWanDriver();
  virtual void setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
  virtual uint64_t getSentCounterPeriodicMessage();
  virtual bool isReadyToRead();
  virtual uint64_t readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer);
  virtual void loopOnce();
  void setLoraWanConfigAdapter(ILoraWanConfigAdapter* loraWanConfigAdapter);
  ILoraWanConfigAdapter* loraWanConfigAdapter();
  void setLoraWanRxDataEventAdapter(ILoraWanRxDataEventAdapter* loraWanRxDataEventAdapter);
  ILoraWanRxDataEventAdapter* loraWanRxDataEventAdapter();

  virtual void configure(bool isForced = false) = 0;

  void setIsSingleChannel(bool isSingleChannel = true);
  bool getIsSinglechannel();

  unsigned int getTxInterval();

  DbgCli_Topic* getCliTopic() { return m_dbgCliTopic; }
  DbgTrace_Port* trPort();
  static LoRaWanDriver* getLoRaWanDriver();

private:
  ILoraWanConfigAdapter* m_loraWanConfigAdapter;
  ILoraWanRxDataEventAdapter* m_loraWanRxDataEventAdapter;
  DbgTrace_Port* m_trPort;
  DbgCli_Topic* m_dbgCliTopic;
  LoRaWanDbgCmd_Configure* m_dbgCliLoRaCfg;
  LoRaWanDbgCmd_SingleChannel* m_dbgCliSingleChannel;
  bool m_isSingleChannel;
  unsigned int m_txInterval;                                  /// LoRaWan transmit interval [s]
  static LoRaWanDriver* s_loRaWanDriver;
  const static unsigned int s_defaultTxInterval;

private: // forbidden default functions
  LoRaWanDriver& operator = (const LoRaWanDriver& src); // assignment operator
  LoRaWanDriver(const LoRaWanDriver& src);              // copy constructor
};
#endif
