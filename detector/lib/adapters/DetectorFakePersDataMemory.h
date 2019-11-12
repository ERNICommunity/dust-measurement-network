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


  static const unsigned int s_numDevices;

private:
  unsigned int sizeOfDeviceKeyStorage();

private:
  unsigned char m_deviceSerialNr;

private: // forbidden default functions
  DetectorFakePersDataMemory& operator = (const DetectorFakePersDataMemory& src); // assignment operator
  DetectorFakePersDataMemory(const DetectorFakePersDataMemory& src);              // copy constructor
};


#endif /* LIB_DETECTOR_IVM_DETECTORIVMMEMORY_H_ */
