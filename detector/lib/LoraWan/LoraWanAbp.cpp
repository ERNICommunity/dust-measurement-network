#include <Arduino.h>
#include <LoraWanAbp.h>
#undef max
#undef min
#include <lmic.h>

#include <SPI.h>
#include <ILoraWanConfigAdapter.h>

#include <ILoraWanRxDataEventAdapter.h>

#include <DbgTracePort.h>
#include <DbgTraceLevel.h>


// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

static uint8_t * m_DataToSend = 0;
static uint64_t m_DataToSendSize = 0;
uint64_t m_CounterPeriodicMessage = 0;

static uint8_t * m_DataToRead = 0;
static uint64_t m_DataToReadSize = 0;


static osjob_t sendjob;

void allocateNewBufferAndDeleteOld(uint8_t** a_Buffer, uint64_t a_Length)
{
  if (a_Buffer != 0)
  {
    *a_Buffer = (uint8_t*) realloc(*a_Buffer, a_Length * sizeof(uint8_t));
  }
  else
  {
    *a_Buffer = (uint8_t*) malloc((int) a_Length * sizeof(uint8_t));
  }
}

void do_send(osjob_t* j)
{
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else
  {
    if (m_DataToSendSize > 0)
    {
      // Prepare upstream data transmission at the next possible time.
      LMIC_setTxData2(1, (xref2u1_t) m_DataToSend, sizeof(uint8_t) * (m_DataToSendSize), 0);
    }
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent(ev_t ev)
{
  LoRaWanDriver* loRaWanDriver = LoRaWanDriver::getLoRaWanDriver();
  ILoraWanRxDataEventAdapter* loRaWanRxDataEventAdapter = loRaWanDriver->loraWanRxDataEventAdapter();
  DbgTrace_Port* trPort = loRaWanDriver->trPort();

  TR_PRINTF(trPort, DbgTrace_Level::debug, "LoRaWan onEvent() start (os_getTime() [hal ticks]: %d).", os_getTime());

  switch (ev)
  {
    case EV_SCAN_TIMEOUT:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_SCAN_TIMEOUT");
      break;
    case EV_BEACON_FOUND:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_BEACON_FOUND");
      break;
    case EV_BEACON_MISSED:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_BEACON_MISSED");
      break;
    case EV_BEACON_TRACKED:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_BEACON_TRACKED");
      break;
    case EV_JOINING:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_JOINING");
      break;
    case EV_JOINED:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_JOINED");
      break;
      /*
       || This event is defined but not used in the code. No
       || point in wasting codespace on it.
       ||
       || case EV_RFU1:
       ||     Serial.println(F("EV_RFU1"));
       ||     break;
       */
    case EV_JOIN_FAILED:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_JOIN_FAILED");
      break;
    case EV_REJOIN_FAILED:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_REJOIN_FAILED");
      break;
    case EV_TXCOMPLETE:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_TXCOMPLETE (includes waiting for RX windows)");
      m_CounterPeriodicMessage++;
      if (LMIC.txrxFlags & TXRX_ACK)
        TR_PRINT_STR(trPort, DbgTrace_Level::debug, "Received ack");
      if (LMIC.dataLen > 0)
      {
        TR_PRINTF(trPort, DbgTrace_Level::notice, "Received %d bytes", LMIC.dataLen);
        allocateNewBufferAndDeleteOld(&m_DataToRead, LMIC.dataLen);
        memcpy(m_DataToRead, &LMIC.frame[LMIC.dataBeg], LMIC.dataLen);
        m_DataToReadSize = LMIC.dataLen;

        char singleBuf[5];
        char strBuf[3*LMIC.dataLen];
        strcpy(strBuf, "");
        for (int i = 0; i < LMIC.dataLen; i++)
        {
          sprintf(singleBuf, "%0X%s", LMIC.frame[LMIC.dataBeg + i], (i != LMIC.dataLen - 1) ? " " : "");
          strcat(strBuf, singleBuf);
        }
        TR_PRINTF(trPort, DbgTrace_Level::notice, "-----> %s", strBuf);
        if (loRaWanRxDataEventAdapter != 0)
        {
          loRaWanRxDataEventAdapter->messageReceived(m_DataToRead, m_DataToReadSize);
        }
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(loRaWanDriver->getTxInterval()), do_send);
      break;
    case EV_LOST_TSYNC:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_LOST_TSYNC");
      break;
    case EV_RESET:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_RESET");
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_RXCOMPLETE");
      break;
    case EV_LINK_DEAD:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_LINK_DEAD");
      break;
    case EV_LINK_ALIVE:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_LINK_ALIVE");
      break;
      /*
       || This event is defined but not used in the code. No
       || point in wasting codespace on it.
       ||
       || case EV_SCAN_FOUND:
       ||    Serial.println(F("EV_SCAN_FOUND"));
       ||    break;
       */
    case EV_TXSTART:
      TR_PRINT_STR(trPort, DbgTrace_Level::debug, "EV_TXSTART");
      break;
    default:
      TR_PRINTF(trPort, DbgTrace_Level::debug, "Unknown event: %u", (unsigned int) ev);
      break;
  }

  TR_PRINTF(trPort, DbgTrace_Level::debug, "LoRaWan onEvent() end.");
}

void configuration()
{
  LoRaWanDriver* loRaWanDriver = LoRaWanDriver::getLoRaWanDriver();
  ILoraWanConfigAdapter* configAdapter = loRaWanDriver->loraWanConfigAdapter();
  DbgTrace_Port* trPort = loRaWanDriver->trPort();

  TR_PRINTF(trPort, DbgTrace_Level::info, "LoRaWan configuration() start.");

#ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
#endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.

    if (0 != configAdapter)
    {
      uint8_t appSKey[16];
      uint8_t nwkSKey[16];

      configAdapter->getAppSKey(appSKey, sizeof(appSKey));
      configAdapter->getNwkSKey(nwkSKey, sizeof(nwkSKey));

      char singleBuf[10];
      char strBuf[8*sizeof(appSKey)];
      strcpy(strBuf, "");
      for (unsigned int i = 0; i < sizeof(appSKey); i++)
      {
        sprintf(singleBuf, "{0x%0X}%s", appSKey[i], (i != sizeof(appSKey) - 1) ? ", " : "");
        strcat(strBuf, singleBuf);
      }

      TR_PRINTF(trPort, DbgTrace_Level::info, "LoRaWanAbp configuration(): AppSKey: %s", strBuf);

      strcpy(strBuf, "");
      for (unsigned int i = 0; i < sizeof(nwkSKey); i++)
      {
        sprintf(singleBuf, "{0x%0X}%s", nwkSKey[i], (i != sizeof(nwkSKey) - 1) ? ", " : "");
        strcat(strBuf, singleBuf);
      }
      TR_PRINTF(trPort, DbgTrace_Level::info, "LoRaWanAbp configuration(): NwkSKey: %s", strBuf);

      TR_PRINTF(trPort, DbgTrace_Level::info, "LoRaWanAbp configuration(): DEVADDR: 0x%X", configAdapter->getDevAddr());

      LMIC_setSession(0x13, configAdapter->getDevAddr(), nwkSKey, appSKey);
    }


    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI); // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK, DR_FSK), BAND_MILLI);   // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7, 14);

    if (loRaWanDriver->getIsSinglechannel())
    {
      // for use with Single channel Gateway only: disable cannels 1..8, just keep channel 0 enabled
      for (int i = 1; i <= 8; i++)
      {
        LMIC_disableChannel(i);
      }
    }

    // Start job
    do_send(&sendjob);

    TR_PRINTF(trPort, DbgTrace_Level::info, "LoRaWan configuration() done.");
}

void loop_once()
{
    unsigned long now;
    now = millis();
    if ((now & 512) != 0)
    {
        digitalWrite(13, HIGH);
    }
    else
    {
        digitalWrite(13, LOW);
    }
    os_runloop_once();
}

LoraWanAbp::LoraWanAbp(ILoraWanConfigAdapter* loraWanConfigAdapter /*= 0*/)
: LoRaWanDriver(loraWanConfigAdapter)
, m_ConnectionIsConfigured(false)
{ }

LoraWanAbp::~LoraWanAbp()
{ }

void LoraWanAbp::configure(bool isForced /*= false*/)
{
  TR_PRINTF(trPort(), DbgTrace_Level::debug, "LoraWanAbp::configure(), isForced: %s", isForced ? "true" : "false");
  if (!m_ConnectionIsConfigured || isForced)
  {
    configuration();
    m_ConnectionIsConfigured = true;
  }
}

bool LoraWanAbp::isReadyToRead()
{
  bool readyToRead = false;
  if(m_DataToRead>0)
  {
      readyToRead = true;
  }
  return readyToRead;
}

uint64_t LoraWanAbp::readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer)
{
  uint64_t bufferSize = a_MaxSizeOfBuffer;
  if (bufferSize > m_DataToReadSize)
  {
    bufferSize = m_DataToReadSize;
  }
  else
  {
    //TODO throw Exception Data buffer is too small for Data
  }
  memcpy(a_Data, m_DataToRead, bufferSize);
  m_DataToRead = (uint8_t*) realloc(m_DataToRead, m_DataToReadSize * sizeof(uint8_t));
  m_DataToReadSize = 0;
  return bufferSize;
}

void LoraWanAbp::setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{
  if (a_SizeOfData > 0)
  {
    // #TODO: add TR_PRINTF debug to show TX data packet
    allocateNewBufferAndDeleteOld(&m_DataToSend, a_SizeOfData);
    memcpy(m_DataToSend, a_Data, sizeof(uint8_t) * (int) a_SizeOfData);
    m_DataToSendSize = a_SizeOfData;
    m_CounterPeriodicMessage = 0;
    if (!m_ConnectionIsConfigured)
    {
      configure();
    }
  }
  else
  {
    TR_PRINTF(trPort(), DbgTrace_Level::error, "ERR: setPeriodicMessageData(): no data to send");
    //TODO SEND EXCEPTION
  }
}

void LoraWanAbp::loopOnce()
{
  loop_once();
}

uint64_t LoraWanAbp::getSentCounterPeriodicMessage()
{
  return m_CounterPeriodicMessage;
}
