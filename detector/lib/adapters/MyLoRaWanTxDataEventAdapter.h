/*
 * MyLoRaWanTxDataEventAdapter.h
 *
 *  Created on: 06.03.2019
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYLORAWANTXDATAEVENTADAPTER_H_
#define LIB_ADAPTERS_MYLORAWANTXDATAEVENTADAPTER_H_

#include <ILoraWanTxDataEventAdapter.h>

class SystemStatusFacade;
class MeasurementFacade;

class MyLoRaWanTxDataEventAdapter: public ILoraWanTxDataEventAdapter
{
public:
  MyLoRaWanTxDataEventAdapter(SystemStatusFacade* systemStatusFacade, MeasurementFacade* measurementFacade);
  virtual ~MyLoRaWanTxDataEventAdapter();

  void messageTransmitted();

private:
  SystemStatusFacade* m_systemStatusFacade;
  MeasurementFacade*  m_measurementFacade;

private: // forbidden default functions
  MyLoRaWanTxDataEventAdapter();                                                    // default constructor
  MyLoRaWanTxDataEventAdapter& operator = (const MyLoRaWanTxDataEventAdapter& src); // assignment operator
  MyLoRaWanTxDataEventAdapter(const MyLoRaWanTxDataEventAdapter& src);              // copy constructor
};

#endif /* LIB_ADAPTERS_MYLORAWANTXDATAEVENTADAPTER_H_ */
