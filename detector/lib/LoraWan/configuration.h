#ifndef _LoraWan_configuration_h_
#define _LoraWan_configuration_h_
#include <Arduino.h>
#undef max
#undef min
#include <lmic.h>
#include <hal/hal.h>
// LoRaWAN NwkSKey, network session key
//static const PROGMEM u1_t NWKSKEY[16] = {0x01,0x23,0x12,0x3D,0x23,0x4A,0x12,0x45,0x01,0x32,0x41,0x32,0x41,0x24,0x31,0x55};

// LoRaWAN AppSKey, application session key
//static const u1_t PROGMEM APPSKEY[16] = {0x01,0x23,0x12,0x3D,0x23,0x4A,0x12,0x45,0x01,0x32,0x41,0x32,0x41,0x24,0x31,0x56};

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
// The library converts the address to network byte order as needed.
//static const u4_t DEVADDR = 0x26011BF3; // <-- Change this address for every node!

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 15;


#endif
