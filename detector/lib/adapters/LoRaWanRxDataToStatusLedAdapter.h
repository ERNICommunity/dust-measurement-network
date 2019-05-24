/*
 * LoRaWanRxDataToStatusLedAdapter.h
 *
 *  Created on: 19.02.2019
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_LORAWANRXDATATOSTATUSLEDADAPTER_H_
#define LIB_ADAPTERS_LORAWANRXDATATOSTATUSLEDADAPTER_H_

#include <ILoraWanRxDataEventAdapter.h>

class ToggleButton;
class LoRaWanDriver;

class LoRaWanRxDataToStatusLedAdapter: public ILoraWanRxDataEventAdapter
{
public:
  LoRaWanRxDataToStatusLedAdapter(ToggleButton* statusLed, LoRaWanDriver* loRaWanDriver);
  virtual ~LoRaWanRxDataToStatusLedAdapter();

  void messageReceived(unsigned char* payload, unsigned int length);

private:
  ToggleButton* m_statusLed;
  LoRaWanDriver* m_loRaWanDriver;

private: // forbidden default functions
  LoRaWanRxDataToStatusLedAdapter& operator = (const LoRaWanRxDataToStatusLedAdapter& src); // assignment operator
  LoRaWanRxDataToStatusLedAdapter(const LoRaWanRxDataToStatusLedAdapter& src);              // copy constructor
  LoRaWanRxDataToStatusLedAdapter();                                                        // default constructor
};

#endif /* LIB_ADAPTERS_LORAWANRXDATATOSTATUSLEDADAPTER_H_ */
