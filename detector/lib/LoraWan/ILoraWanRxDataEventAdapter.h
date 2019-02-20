/*
 * ILoraWanRxDataEventAdapter.h
 *
 *  Created on: 12.02.2019
 *      Author: nid
 */

#ifndef LIB_LORAWAN_ILORAWANRXDATAEVENTADAPTER_H_
#define LIB_LORAWAN_ILORAWANRXDATAEVENTADAPTER_H_


class ILoraWanRxDataEventAdapter
{
public:
  virtual ~ILoraWanRxDataEventAdapter() { }

  virtual void messageReceived(unsigned char* payload, unsigned int length) = 0;

protected:
  ILoraWanRxDataEventAdapter() { }

private: // forbidden default functions
  ILoraWanRxDataEventAdapter& operator = (const ILoraWanRxDataEventAdapter& src); // assignment operator
  ILoraWanRxDataEventAdapter(const ILoraWanRxDataEventAdapter& src);              // copy constructor
};


#endif /* LIB_LORAWAN_ILORAWANRXDATAEVENTADAPTER_H_ */
