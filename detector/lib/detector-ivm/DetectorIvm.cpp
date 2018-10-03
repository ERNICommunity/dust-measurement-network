/*
 * DetectorIvm.cpp
 *
 *  Created on: 26.08.2018
 *      Author: niklaudi
 */

#include "DetectorIvmMemory.h"
//#include <DbgTracePort.h>
//#include <DbgTraceLevel.h>

#include <DetectorIvm.h>

DetectorIvm::DetectorIvm()
: Ivm(new DetectorIvmMemory())
//, m_trPort(new DbgTrace_Port("ivm", DbgTrace_Level::debug))
{
  maintainVersionChange();
}

DetectorIvm::~DetectorIvm()
{
  delete m_trPort;
  m_trPort = 0;

  delete getIvmMemory();
}

unsigned int DetectorIvm::getDeviceId(char* deviceId, unsigned int len)
{
  unsigned int count = 0;
  const unsigned int addr = DetectorIvmMemory::KT_devId * DetectorIvmMemory::s_numMaxChars;
  count = readFromIvm(addr, deviceId, len-1);
  deviceId[count] = '\0';
  return count;
}

unsigned int DetectorIvm::getDevAddr(char* devAddr,  unsigned int len)
{
  unsigned int count = 0;
  const unsigned int addr = DetectorIvmMemory::KT_devAddr * DetectorIvmMemory::s_numMaxChars;
  count = readFromIvm(addr, devAddr, len-1);
  devAddr[count] = '\0';
  return count;
}

unsigned int DetectorIvm::getNwkSKey(char* nwkSKey,  unsigned int len)
{
  unsigned int count = 0;
  const unsigned int addr = DetectorIvmMemory::KT_nwkSKey * DetectorIvmMemory::s_numMaxChars;
  count = readFromIvm(addr, nwkSKey, len-1);
  nwkSKey[count] = '\0';
  return count;
}

unsigned int DetectorIvm::getAppSKey(char* appSKey,  unsigned int len)
{
  unsigned int count = 0;
  const unsigned int addr = DetectorIvmMemory::KT_appSKey * DetectorIvmMemory::s_numMaxChars;
  count = readFromIvm(addr, appSKey, len-1);
  appSKey[count] = '\0';
  return count;
}

void DetectorIvm::maintainVersionChange()
{
  char deviceId[DetectorIvmMemory::s_numMaxChars];
  char devAddr[DetectorIvmMemory::s_numMaxChars];
  char nwkSKey[DetectorIvmMemory::s_numMaxChars];
  char appSKey[DetectorIvmMemory::s_numMaxChars];

  getDeviceId(deviceId, DetectorIvmMemory::s_numMaxChars);
  getDevAddr(devAddr, DetectorIvmMemory::s_numMaxChars);
  getNwkSKey(nwkSKey, DetectorIvmMemory::s_numMaxChars);
  getAppSKey(appSKey, DetectorIvmMemory::s_numMaxChars);


  DetectorIvmMemory* myIvmMemory = static_cast<DetectorIvmMemory*>(getIvmMemory());

//  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "HW DevId: %d", myIvmMemory->getHwDeviceId());
//  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "deviceId: %s", deviceId);
//  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "devAddr:  %s", devAddr);
//  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "nwkSKey:  %s", nwkSKey);
//  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "appSKey:  %s", appSKey);
}
