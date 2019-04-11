/*
 * MyLoRaSysDataandlerAdapter.h
 *
 *  Created on: 01.03.2019
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYSYSTEMSTATUSFACADEADAPTER_H_
#define LIB_ADAPTERS_MYSYSTEMSTATUSFACADEADAPTER_H_

#include <ISystemStatusFacadeAdapter.h>
#include <SystemStatusFacade.h>

class Battery;

class MySystemStatusFacadeAdapter: public ISystemStatusFacadeAdapter
{
public:
  MySystemStatusFacadeAdapter(Battery* battery, SystemStatusFacade* systemStatusFacade);
  virtual ~MySystemStatusFacadeAdapter();

  SystemStatusFacade::State getBatteryState();
  float getBatteryVoltage();

  void updateSystemStatus();

private:
  Battery* m_battery;
  SystemStatusFacade* m_systemStatusFacade;

private: // forbidden default functions
  MySystemStatusFacadeAdapter();                                                 // default constructor
  MySystemStatusFacadeAdapter& operator = (const MySystemStatusFacadeAdapter& src); // assignment operator
  MySystemStatusFacadeAdapter(const MySystemStatusFacadeAdapter& src);              // copy constructor
};

#endif /* LIB_ADAPTERS_MYSYSTEMSTATUSFACADEADAPTER_H_ */
