/*
 * DetectorIvmMemory.cpp
 *
 *  Created on: 27.08.2018
 *      Author: niklaudi
 */

#include <DetectorFakePersDataMemory.h>

const unsigned int DetectorFakePersDataMemory::s_numDevices  = 10;
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
  if (m_deviceSerialNr < s_numDevices)
  {
    const char deviceSpecificKeys[s_numDevices][KT_NumKeys][s_numMaxChars+1] =
    {
      { // ERNI DMN - Node #0
        { "erni-dmn-node-00009876b6106f1e" },    // deviceId
        { "26011B83" },                          // devAddr
        { "D4936ED63314416781EC511CBA7CE2D1" },  // nwkSKey
        { "CCA8C876E2F53A706B5A565B38160931" }   // appSKey
      },
      { // ERNI DMN - Node #1
        { "erni-dmn-node-00009876b6106f27" },    // deviceId
        { "26011120" },                          // devAddr
        { "3FCB918FACD42BAB4D4507FFEE7BE047" },  // nwkSKey
        { "3815692F5E1D19AA75F476BB08887C90" }   // appSKey
      },
      { // ERNI DMN - Node #2
        { "erni-dmn-node-00009876b610bdd2" },    // deviceId
        { "2601194F" },                          // devAddr
        { "30679C1293758F7784DA3896DEC5EF59" },  // nwkSKey
        { "F3E1E27381C4A84E8658C7E9CEE88625" }   // appSKey
      },
      { // ERNI DMN - Node #3
        { "erni-dmn-node-00009876b610bf1f" },    // deviceId
        { "2601169D" },                          // devAddr
        { "DA08547B2AAFC7DC57B784894FD3AFA5" },  // nwkSKey
        { "E9B83B8CADC5CFE8BBC24E39ECA4DC9A" }   // appSKey
      },
      { // ERNI DMN - Node #4
        { "erni-dmn-node-00009876b610bf99" },    // deviceId
        { "26011AA3" },                          // devAddr
        { "82FE5E05A715E0EBFF0875ED58B8027B" },  // nwkSKey
        { "4212FB8F2F5009659134B0BB62885C88" }   // appSKey
      },
      { // ERNI DMN - Node #5
        { "erni-dmn-node-00009876b610ffd6" },    // deviceId
        { "26011797" },                          // devAddr
        { "D8AB4C235FB0F18930F6020EB324D758" },  // nwkSKey
        { "D3967957D41F93728AC21CB9D6ECD5D7" }   // appSKey
      },
      { // ERNI DMN - Node #6
        { "erni-dmn-node-00009876b61108ff" },    // deviceId
        { "26011F63" },                          // devAddr
        { "9F6F30E395EB4AE9DE0B168237B1EF26" },  // nwkSKey
        { "ADA3FFD4E35A4DC8C6CD1921FE1DDE89" }   // appSKey
      },
      { // ERNI DMN - Node #7
        { "erni-dmn-node-00009876b6110978" },    // deviceId
        { "26011A5C" },                          // devAddr
        { "E907ED9495F9A9FD162DE1133317CA33" },  // nwkSKey
        { "E63343BE703656C18B0A68AF4EBB4D3E" }   // appSKey
      },
      { // ERNI DMN - Node #8
        { "erni-dmn-node-00009876b61109d7" },    // deviceId
        { "260117E4" },                          // devAddr
        { "2CE7F7D854E020A3C9DA74F520F32131" },  // nwkSKey
        { "5F334B97C9A2BD7C8EF06EE6AE99FAEB" }   // appSKey
      },
      { // ERNI DMN - Node #9
        { "erni-dmn-node-00009876b6110a1a" },    // deviceId
        { "260112A5" },                          // devAddr
        { "06DA81B8E092474802149D5DEFD560ED" },  // nwkSKey
        { "966E298ACFA624CED202FA9C14A92C16" }   // appSKey
      }
    };
    unsigned int key = address / s_numMaxChars;
    unsigned int ch  = address % s_numMaxChars;
    return deviceSpecificKeys[m_deviceSerialNr][key][ch];
  }
  else
  {
    return 0;
  }
}

void DetectorFakePersDataMemory::setDeviceSerialNr(unsigned long int deviceSerialNr)
{
  m_deviceSerialNr = deviceSerialNr;
}

unsigned int DetectorFakePersDataMemory::sizeOfDeviceKeyStorage()
{
  return s_numDevices * KT_NumKeys * s_numMaxChars;
}
