/*
 * ILoraWanConfigAdapter.h
 *
 *  Created on: 19.11.2018
 *      Author: NID0102
 */

#ifndef LIB_LORAWAN_ILORAWANCONFIGADAPTER_H_
#define LIB_LORAWAN_ILORAWANCONFIGADAPTER_H_

class ILoraWanConfigAdapter
{
public:
  virtual ~ILoraWanConfigAdapter() { }

  /**
   * Read Device Address string.
   * @param devAddr OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Device Address
   * @return Number of characters read (including '\0' terminator character)
   */
  virtual unsigned int getDevAddr(  char* devAddr,  unsigned int len) = 0;

  /**
   * Read Network Security Key string.
   * @param Network Security Key OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Network Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  virtual unsigned int getNwkSKey( char* nwkSKey,  unsigned int len) = 0;

  /**
   * Read Application Security Key string.
   * @param Application Security Key OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Application Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  virtual unsigned int getAppSKey(  char* appSKey,  unsigned int len) = 0;

protected:
  ILoraWanConfigAdapter() { }

private: // forbidden default functions
  ILoraWanConfigAdapter& operator = (const ILoraWanConfigAdapter& src); // assignment operator
  ILoraWanConfigAdapter(const ILoraWanConfigAdapter& src);              // copy constructor
};

#endif /* LIB_LORAWAN_ILORAWANCONFIGADAPTER_H_ */
