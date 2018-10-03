/*
 * DetectorIvmMemory.cpp
 *
 *  Created on: 27.08.2018
 *      Author: niklaudi
 */

//#include <Arduino.h>

#include <DetectorIvmMemory.h>

const unsigned int DetectorIvmMemory::s_numDevices  =  5;
const unsigned int DetectorIvmMemory::s_numMaxChars = 32;

//-----------------------------------------------------------------------------
DetectorIvmMemory::DetectorIvmMemory()
: m_hwDeviceId(0)
{
  // read the 3 Bit HW Device Id
  m_hwDeviceId = getHwDeviceId();
}

DetectorIvmMemory::~DetectorIvmMemory()
{ }

void DetectorIvmMemory::write(unsigned int address, unsigned char data)
{
  // Dummy write - no action
}

unsigned char DetectorIvmMemory::read(unsigned int address)
{
  const unsigned char deviceSpecificKeys[s_numDevices][KT_NumKeys][s_numMaxChars+1] =
  {
    {
      { "dust-node-00009876b6106f1e______" },  // deviceId
      { "26011430________________________" },  // devAddr
      { "5BBB9FDB3FDC2D831B58DD5F0574B57A" },  // nwkSKey
      { "8072354DD74768A5AD26DC819741FBEB" }   // appSKey
    },
    {
      { "dust-node-00009876b6106f27______" },
      { "26011F80________________________" },
      { "789BB7541EC24753EBD3BF8601BB198C" },
      { "4630C03F65B37C6CC538C96710C8B8F8" }
    },
    {
      { "dust-node-00009876b610bdd2______" },
      { "26011A76________________________" },
      { "6B0EE00A7C0C95A361B2ADA8B63039D3" },
      { "8621804E68BE994C66758DF6B300DD99" }
    },
    {
      { "dust-node-00009876b610bf1f______" },
      { "26011817________________________" },
      { "66D75E115A3021423782BF15D8D9CAC2" },
      { "2D54BD5203FC18D272A8E9F1B56439B1" }
    },
    {
      { "dust-node-00009876b610bf99______" },
      { "260110CA________________________" },
      { "28600BE3147B28227FB254CAFC58C568" },
      { "3F6EC124E9A08EC6BB38FB5404055A7E" }
    }
  };
  unsigned int key = address / s_numMaxChars;
  unsigned int ch  = address % s_numMaxChars + address / s_numMaxChars;
  return deviceSpecificKeys[m_hwDeviceId][key][ch];
}

unsigned char DetectorIvmMemory::getHwDeviceId()
{
  unsigned char hwDeviceId = 0;
//  hwDeviceId  = digitalRead(9);
//  hwDeviceId += digitalRead(10) << 1;
//  hwDeviceId += digitalRead(11) << 2;
  return hwDeviceId;
}

unsigned char DetectorIvmMemory::hwDeviceId()
{
  return m_hwDeviceId;
}

unsigned int DetectorIvmMemory::sizeOfDeviceKeyStorage()
{
  return s_numDevices * KT_NumKeys * s_numMaxChars;
}
