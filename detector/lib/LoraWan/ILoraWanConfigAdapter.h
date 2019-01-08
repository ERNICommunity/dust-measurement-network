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
   * Read Device Address.
   * @return Device Address
   */
  virtual unsigned int getDevAddr() = 0;

  /**
   * Read Network Session Key byte array.
    * @param nwkSKey Network Session Key OUT: Buffer for Network Session Key byte array
   * @param len Buffer size for Network Session Key
   */
  virtual void getNwkSKey(unsigned char* nwkSKey, unsigned int len) = 0;

  /**
   * Read Application Session Key byte array.
    * @param appSKey Application Session Key OUT: Buffer for Application Session Key byte array
   * @param len Buffer size for Application Session Key
   */
  virtual void getAppSKey(unsigned char* appSKey, unsigned int len) = 0;

protected:
  ILoraWanConfigAdapter() { }

private: // forbidden default functions
  ILoraWanConfigAdapter& operator = (const ILoraWanConfigAdapter& src); // assignment operator
  ILoraWanConfigAdapter(const ILoraWanConfigAdapter& src);              // copy constructor
};

#endif /* LIB_LORAWAN_ILORAWANCONFIGADAPTER_H_ */
