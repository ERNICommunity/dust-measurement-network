/*
 * MyBatteryAdapter.h
 *
 *  Created on: 15.03.2018
 *      Author: niklaudi
 */

#ifndef LIB_ADAPTERS_MYBATTERYADAPTER_H_
#define LIB_ADAPTERS_MYBATTERYADAPTER_H_

#include <Battery.h>

class MyBatteryAdapter: public BatteryAdapter
{
public:
  MyBatteryAdapter();
  virtual ~MyBatteryAdapter();

  unsigned int readRawBattSenseValue();

private: // forbidden default functions
	MyBatteryAdapter& operator = (const MyBatteryAdapter& src); // assignment operator
	MyBatteryAdapter(const MyBatteryAdapter& src);              // copy constructor
};

#endif /* LIB_ADAPTERS_MYBATTERYADAPTER_H_ */
