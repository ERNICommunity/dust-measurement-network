/*
 * DetectorIvmMemory.h
 *
 *  Created on: 27.08.2018
 *      Author: niklaudi
 */

#ifndef LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_
#define LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_

#include <IF_IvmMemory.h>

class DetectorIvmMemory : public IF_IvmMemory
{
public:
  DetectorIvmMemory();
  virtual ~DetectorIvmMemory();

  void write(unsigned int address, unsigned char data);
  unsigned char read(unsigned int address);

  void maintainVersionChange() { }
  unsigned char getHwDeviceId();

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
  unsigned char hwDeviceId();
  unsigned int sizeOfDeviceKeyStorage();

private:
  unsigned char m_hwDeviceId;

private: // forbidden default functions
  DetectorIvmMemory& operator = (const DetectorIvmMemory& src); // assignment operator
  DetectorIvmMemory(const DetectorIvmMemory& src);              // copy constructor
};


#endif /* LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_ */
