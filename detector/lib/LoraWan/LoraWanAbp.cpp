#include "LoraWanAbp.hpp"
#include <Arduino.h>
#undef max
#undef min
#include <lmic.h>

#include <SPI.h>
#include "configuration.h"

#include <hal/hal.h>

// Pin mapping
// Adapted for Feather M0 per p.10 of [feather]
const lmic_pinmap lmic_pins = {
    .nss = 8,                       // chip select on feather (rf95module) CS
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,                       // reset pin
    .dio = {6, 3, LMIC_UNUSED_PIN}, // assumes external jumpers [feather_lora_jumper]
                                    // DIO1 is on JP1-1: is io1 - we connect to GPO6
                                    // DIO1 is on JP5-3: is D2 - we connect to GPO5
};


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
    if (a_Buffer != 0) {
        *a_Buffer = (uint8_t*) realloc(*a_Buffer, a_Length * sizeof(uint8_t));
    }
    else{
        *a_Buffer = (uint8_t*) malloc((int)a_Length * sizeof(uint8_t));
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
        if(m_DataToSendSize>0)
        {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, (xref2u1_t)m_DataToSend, sizeof(uint8_t)*(m_DataToSendSize), 0);
        Serial.println(F("Packet queued"));
        }
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent(ev_t ev)
{
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev)
    {
    case EV_SCAN_TIMEOUT:
        Serial.println(F("EV_SCAN_TIMEOUT"));
        break;
    case EV_BEACON_FOUND:
        Serial.println(F("EV_BEACON_FOUND"));
        break;
    case EV_BEACON_MISSED:
        Serial.println(F("EV_BEACON_MISSED"));
        break;
    case EV_BEACON_TRACKED:
       Serial.println(F("EV_BEACON_TRACKED"));
        break;
    case EV_JOINING:
        Serial.println(F("EV_JOINING"));
        break;
    case EV_JOINED:
        Serial.println(F("EV_JOINED"));
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
        Serial.println(F("EV_JOIN_FAILED"));
        break;
    case EV_REJOIN_FAILED:
        Serial.println(F("EV_REJOIN_FAILED"));
        break;
    case EV_TXCOMPLETE:
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        m_CounterPeriodicMessage++;
        if (LMIC.txrxFlags & TXRX_ACK)
            Serial.println(F("Received ack"));
        if (LMIC.dataLen>0)
        {
            Serial.println(F("Received "));
            Serial.println(LMIC.dataLen);
            Serial.print("-----> ");
            allocateNewBufferAndDeleteOld(&m_DataToRead,LMIC.dataLen);
            memcpy(m_DataToRead,&LMIC.frame[LMIC.dataBeg],LMIC.dataLen);
            m_DataToReadSize = LMIC.dataLen;
            for (int i = 0; i < LMIC.dataLen; i++)
            {
                Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
            }
            Serial.println(F(" bytes of payload"));
        }
        // Schedule next transmission
        // os_setTimedCallbackos_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
        break;
    case EV_LOST_TSYNC:
        Serial.println(F("EV_LOST_TSYNC"));
        break;
    case EV_RESET:
        Serial.println(F("EV_RESET"));
        break;
    case EV_RXCOMPLETE:
        // data received in ping slot
        Serial.println(F("EV_RXCOMPLETE"));
        break;
    case EV_LINK_DEAD:
        Serial.println(F("EV_LINK_DEAD"));
        break;
    case EV_LINK_ALIVE:
        Serial.println(F("EV_LINK_ALIVE"));
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
        Serial.println(F("EV_TXSTART"));
        break;
    default:
        Serial.print(F("Unknown event: "));
        Serial.println((unsigned)ev);
        break;
    }
}

void configuration()
{
       // pinMode(13, OUTPUT);
    while (!Serial); // wait for Serial to be initialized
    Serial.begin(115200);
    delay(1000); // per sample code on RF_95 test
    Serial.println(F("Starting"));

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
#ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession(0x13, DEVADDR, nwkskey, appskey);
#else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession(0x13, DEVADDR, NWKSKEY, APPSKEY);
#endif

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

    // Start job
    do_send(&sendjob);
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

LoraWanAbp::LoraWanAbp():
    m_ConnectionIsConfigured(false)
{

}

void LoraWanAbp::configure()
{
    if(!m_ConnectionIsConfigured)
    {
        configuration();
        m_ConnectionIsConfigured = true;
    }
    else{
        //TODO SEND EXCEPTION
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
    if(bufferSize>m_DataToReadSize)
    {
        bufferSize = m_DataToReadSize;
    }
    else{
        //TODO throw Exception Data buffer is to small for Data
    }
    memcpy(a_Data,m_DataToRead,bufferSize);
    m_DataToRead = (uint8_t*) realloc(m_DataToRead, m_DataToReadSize * sizeof(uint8_t));
    m_DataToReadSize=0;
    return bufferSize;
}
 

void LoraWanAbp::setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData)
{
    if(a_SizeOfData>0)
    {
        allocateNewBufferAndDeleteOld(&m_DataToSend,a_SizeOfData);
        memcpy(m_DataToSend,a_Data, sizeof(uint8_t)*(int)a_SizeOfData);
        m_DataToSendSize = a_SizeOfData;
        m_CounterPeriodicMessage = 0;
        if(!m_ConnectionIsConfigured)
        {
            configure();
        }
    }
    else{
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