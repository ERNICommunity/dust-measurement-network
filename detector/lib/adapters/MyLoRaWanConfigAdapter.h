/*
 * MyLoRaWanConfigAdapter.h
 *
 *  Created on: 24.11.2018
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYLORAWANCONFIGADAPTER_H_
#define LIB_ADAPTERS_MYLORAWANCONFIGADAPTER_H_

#include <ILoraWanConfigAdapter.h>

class Assets;

class MyLoRaWanConfigAdapter: public ILoraWanConfigAdapter
{
public:
  MyLoRaWanConfigAdapter(Assets* assets);
  virtual ~MyLoRaWanConfigAdapter();

  /**
   * Read Device Address.
   * @return Device Address
   */
  unsigned int getDevAddr();

  /**
   * Read Network Session Key byte array.
   * @param nwkSKey Network Session Key OUT: Buffer for Network Session Key byte array
   * @param len Buffer size for Network Session Key
   * @return Number of bytes read
   */
  void getNwkSKey(unsigned char* nwkSKey, unsigned int len);

  /**
   * Read Application Session Key byte array.
   * @param appSKey Application Session Key OUT: Buffer for Application Session Key byte array
   * @param len Buffer size for Application Session Key
   * @return Number of bytes read
   */
  void getAppSKey(unsigned char* appSKey, unsigned int len);

private:
  /**
   * Convert a LoRaWan Session Key string into a byte array.
   * @param sessionKeyBytes OUT: Session Key byte array
   * @param len Size of the byte array
   * @param sessionKeyString IN: Session Key string (string size (excl. null termination char) is assumed to be twice te size as set by len.
   */
  void convertStringToByteArray(unsigned char* sessionKeyBytes, unsigned int len, const char* sessionKeyString);

private:
  Assets* m_assets;

private: // forbidden default functions
  MyLoRaWanConfigAdapter& operator = (const MyLoRaWanConfigAdapter& src); // assignment operator
  MyLoRaWanConfigAdapter(const MyLoRaWanConfigAdapter& src);              // copy constructor
  MyLoRaWanConfigAdapter();                                               // default constructor
};

#endif /* LIB_ADAPTERS_MYLORAWANCONFIGADAPTER_H_ */
