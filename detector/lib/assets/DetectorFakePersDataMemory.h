/*
 * DetectorIvmMemory.h
 *
 *  Created on: 27.08.2018
 *      Author: niklaudi
 */

#ifndef LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_
#define LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_

#include <IPersistentDataMemory.h>

class DetectorFakePersDataMemory : public IPersistentDataMemory
{
public:
  DetectorFakePersDataMemory();
  virtual ~DetectorFakePersDataMemory();

  void write(unsigned int address, unsigned char data);
  char read(unsigned int address);

  void maintainVersionChange() { }
  void setDeviceSerialNr(unsigned long int deviceSerialNr);


public:
  typedef enum
  {
    KT_devId   = 0,
    KT_devAddr = 1,
    KT_nwkSKey = 2,
    KT_appSKey = 3,
    KT_NumKeys
  } KeyType;

  static const unsigned int s_numDevices;
  static const unsigned int s_numMaxChars;

private:
  unsigned int sizeOfDeviceKeyStorage();

private:
  unsigned char m_deviceSerialNr;

private: // forbidden default functions
  DetectorFakePersDataMemory& operator = (const DetectorFakePersDataMemory& src); // assignment operator
  DetectorFakePersDataMemory(const DetectorFakePersDataMemory& src);              // copy constructor
};


#endif /* LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_ */
