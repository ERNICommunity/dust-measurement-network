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
: m_deviceSerialNr(0)
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
    { /* device HW serial 0 */
      { "erni-dmn-node-00009876b6106f1e" },    // deviceId
      { "26011B83" },                          // devAddr
      { "D4936ED63314416781EC511CBA7CE2D1" },  // nwkSKey
      { "CCA8C876E2F53A706B5A565B38160931" }   // appSKey
    },
    { /* device HW serial 1 */
      { "erni-dmn-node-00009876b6106f27" },    // deviceId
      { "26011120" },                          // devAddr
      { "3FCB918FACD42BAB4D4507FFEE7BE047" },  // nwkSKey
      { "3815692F5E1D19AA75F476BB08887C90" }   // appSKey
    },
    { /* device HW serial 2 */
      { "erni-dmn-node-00009876b610bdd2" },    // deviceId
      { "2601194F" },                          // devAddr
      { "30679C1293758F7784DA3896DEC5EF59" },  // nwkSKey
      { "F3E1E27381C4A84E8658C7E9CEE88625" }   // appSKey
    },
    { /* device HW serial 3 */
      { "erni-dmn-node-00009876b610bf1f" },    // deviceId
      { "2601169D" },                          // devAddr
      { "DA08547B2AAFC7DC57B784894FD3AFA5" },  // nwkSKey
      { "E9B83B8CADC5CFE8BBC24E39ECA4DC9A" }   // appSKey
    },
    { /* device HW serial 4 */
      { "erni-dmn-node-00009876b610bf99" },    // deviceId
      { "26011AA3" },                          // devAddr
      { "82FE5E05A715E0EBFF0875ED58B8027B" },  // nwkSKey
      { "4212FB8F2F5009659134B0BB62885C88" }   // appSKey
    }
  };
  unsigned int key = address / s_numMaxChars;
  unsigned int ch  = address % s_numMaxChars;
  return deviceSpecificKeys[m_deviceSerialNr][key][ch];
}

void DetectorFakePersDataMemory::setDeviceSerialNr(unsigned long int deviceSerialNr)
{
  m_deviceSerialNr = deviceSerialNr;
}

unsigned int DetectorFakePersDataMemory::sizeOfDeviceKeyStorage()
{
  return s_numDevices * KT_NumKeys * s_numMaxChars;
}
