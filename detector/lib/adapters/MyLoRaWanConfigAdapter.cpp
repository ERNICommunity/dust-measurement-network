/*
 * MyLoRaWanConfigAdapter.cpp
 *
 *  Created on: 24.11.2018
 *      Author: nid
 */

#include <Assets.h>
#include <string.h>
#include <stdlib.h>

#include <MyLoRaWanConfigAdapter.h>

MyLoRaWanConfigAdapter::MyLoRaWanConfigAdapter(Assets* assets)
: ILoraWanConfigAdapter()
, m_assets(assets)
{ }

MyLoRaWanConfigAdapter::~MyLoRaWanConfigAdapter()
{ }

unsigned int MyLoRaWanConfigAdapter::getDevAddr()
{
  unsigned int devAddr = 0x26011BF3;
  if (0 != m_assets)
  {
    const unsigned int len = 2 * sizeof(unsigned int) + 1;
    char devAddrStr[len]; /* = "26011BF3";*/
    m_assets->getDevAddr(devAddrStr, len);
    devAddr = static_cast<unsigned int>(strtol(devAddrStr, 0, 16));
  }
  return devAddr;
}

void MyLoRaWanConfigAdapter::getNwkSKey(unsigned char* nwkSKey, unsigned int len)
{
  char nwkSKeyStr[2*len+1]; /*= "0123123D234A12450132413241243155";*/
  // static const PROGMEM u1_t NWKSKEY[16] = {0x01,0x23,0x12,0x3D,0x23,0x4A,0x12,0x45,0x01,0x32,0x41,0x32,0x41,0x24,0x31,0x55};

  if (0 != m_assets)
  {
    m_assets->getNwkSKey(nwkSKeyStr, 2*len+1);
  }
  convertStringToByteArray(nwkSKey, len, nwkSKeyStr);
}

void MyLoRaWanConfigAdapter::getAppSKey(unsigned char* appSKey, unsigned int len)
{
  char appSKeyStr[2*len+1]; /* = "0123123D234A12450132413241243156";*/
  // static const u1_t PROGMEM APPSKEY[16] = {0x01,0x23,0x12,0x3D,0x23,0x4A,0x12,0x45,0x01,0x32,0x41,0x32,0x41,0x24,0x31,0x56};

  if (0 != m_assets)
  {
    m_assets->getAppSKey(appSKeyStr, 2*len+1);
  }
  convertStringToByteArray(appSKey, len, appSKeyStr);
}

void MyLoRaWanConfigAdapter::convertStringToByteArray(unsigned char* sessionKeyBytes, unsigned int len, const char* sessionKeyString)
{
  unsigned int strLen = 2 * len;

  for (unsigned int i = 0; i < strLen; i+=2)
  {
    char substr[3];
    const char* strSel = &(sessionKeyString[i]);
    strncpy(substr, strSel, 2);
    substr[2] = '\0';
    sessionKeyBytes[i/2] = static_cast<unsigned int>(strtol(substr, 0, 16));
  }
}
