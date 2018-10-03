/*
 * DetectorIvm.h
 *
 *  Created on: 26.08.2018
 *      Author: niklaudi
 */

#ifndef LIB_DETECTOR_IVM_DETECTORIVM_H_
#define LIB_DETECTOR_IVM_DETECTORIVM_H_

#include <Ivm.h>

class DbgTrace_Port;

class DetectorIvm : public Ivm
{
public:
  DetectorIvm();
  virtual ~DetectorIvm();

  /**
   * Read Device Id string.
   * @param deviceId OUT: Buffer for Device Id string to read from IVM Memory.
   * @param len Buffer size for Device Id
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getDeviceId( char* deviceId, unsigned int len);

  /**
   * Read Device Address string.
   * @param devAddr OUT: Buffer for Device Address string to read from IVM Memory.
   * @param len Buffer size for Device Address
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getDevAddr(  char* devAddr,  unsigned int len);

  /**
   * Read Network Security Key string.
   * @param Network Security Key OUT: Buffer for Device Address string to read from IVM Memory.
   * @param len Buffer size for Network Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getNwkSKey( char* nwkSKey,  unsigned int len);

  /**
   * Read Application Security Key string.
   * @param Application Security Key OUT: Buffer for Device Address string to read from IVM Memory.
   * @param len Buffer size for Application Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getAppSKey(  char* appSKey,  unsigned int len);

protected:
  void maintainVersionChange();

private:
//  DbgTrace_Port* m_trPort;

private: // forbidden default functions
  DetectorIvm& operator = (const DetectorIvm& src); // assignment operator
  DetectorIvm(const DetectorIvm& src);              // copy constructor
};

#endif /* LIB_DETECTOR_IVM_DETECTORIVM_H_ */
