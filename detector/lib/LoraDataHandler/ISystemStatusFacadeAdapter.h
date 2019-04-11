/*
 * ILoRaSysDataHandlerAdapter.h
 *
 *  Created on: 01.03.2019
 *      Author: nid
 */

#ifndef LIB_LORADATAHANDLER_ISYSTEMSTATUSFACADEADAPTER_H_
#define LIB_LORADATAHANDLER_ISYSTEMSTATUSFACADEADAPTER_H_

#include <SystemStatusFacade.h>

class ISystemStatusFacadeAdapter
{
public:
  virtual ~ISystemStatusFacadeAdapter() { }

  virtual SystemStatusFacade::State getBatteryState() = 0;
  virtual float getBatteryVoltage() = 0;

protected:
  ISystemStatusFacadeAdapter() { }

private: // forbidden default functions
  ISystemStatusFacadeAdapter& operator = (const ISystemStatusFacadeAdapter& src); // assignment operator
  ISystemStatusFacadeAdapter(const ISystemStatusFacadeAdapter& src);              // copy constructor
};

#endif /* LIB_LORADATAHANDLER_ISYSTEMSTATUSFACADEADAPTER_H_ */
