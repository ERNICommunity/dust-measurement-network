/*
 * DetectorIvmMemory.cpp
 *
 *  Created on: 27.08.2018
 *      Author: niklaudi
 */

//#include <Arduino.h>

#include <DetectorFakePersDataMemory.h>

const unsigned int DetectorFakePersDataMemory::s_numDevices  =  6;
const unsigned int DetectorFakePersDataMemory::s_numMaxChars = 32;

//-----------------------------------------------------------------------------
DetectorFakePersDataMemory::DetectorFakePersDataMemory()
: m_hwDeviceId(0)
{ }

DetectorFakePersDataMemory::~DetectorFakePersDataMemory()
{ }

void DetectorFakePersDataMemory::write(unsigned int address, unsigned char data)
{
  // Dummy write - no action
}

char DetectorFakePersDataMemory::read(unsigned int address)
{
  const char deviceSpecificKeys[s_numDevices][KT_NumKeys][s_numMaxChars+1] =
  {
    {
      { "erni-dustnode0809099878656515" },     // deviceId
      { "26011BF3" },                          // devAddr
      { "0123123D234A12450132413241243155" },  // nwkSKey
      { "0123123D234A12450132413241243156" }   // appSKey
    },
    {
      { "dust-node-00009876b6106f1e" },        // deviceId
      { "26011430" },                          // devAddr
      { "5BBB9FDB3FDC2D831B58DD5F0574B57A" },  // nwkSKey
      { "8072354DD74768A5AD26DC819741FBEB" }   // appSKey
    },
    {
      { "dust-node-00009876b6106f27" },
      { "26011F80" },
      { "789BB7541EC24753EBD3BF8601BB198C" },
      { "4630C03F65B37C6CC538C96710C8B8F8" }
    },
    {
      { "dust-node-00009876b610bdd2" },
      { "26011A76" },
      { "6B0EE00A7C0C95A361B2ADA8B63039D3" },
      { "8621804E68BE994C66758DF6B300DD99" }
    },
    {
      { "dust-node-00009876b610bf1f" },
      { "26011817" },
      { "66D75E115A3021423782BF15D8D9CAC2" },
      { "2D54BD5203FC18D272A8E9F1B56439B1" }
    },
    {
      { "dust-node-00009876b610bf99" },
      { "260110CA" },
      { "28600BE3147B28227FB254CAFC58C568" },
      { "3F6EC124E9A08EC6BB38FB5404055A7E" }
    }
  };
  unsigned int key = address / s_numMaxChars;
  unsigned int ch  = address % s_numMaxChars;
  return deviceSpecificKeys[m_hwDeviceId][key][ch];
}

void DetectorFakePersDataMemory::setDeviceSerialNr(unsigned long int deviceSerialNr)
{
  m_hwDeviceId = deviceSerialNr;
}

unsigned int DetectorFakePersDataMemory::sizeOfDeviceKeyStorage()
{
  return s_numDevices * KT_NumKeys * s_numMaxChars;
}
