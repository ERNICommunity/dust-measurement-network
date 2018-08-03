/*
 * MyTemperatureHumidityAdapter.h
 *
 *  Created on: 29.08.2017
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYTEMPERATUREHUMIDITYADAPTER_H_
#define LIB_ADAPTERS_MYTEMPERATUREHUMIDITYADAPTER_H_

#include <DHT_Process.h>

class MyDHT_ProcessAdapter : public DHT_ProcessAdapter
{
public:
  MyDHT_ProcessAdapter();
  virtual ~MyDHT_ProcessAdapter();

  void notifyTemperatureChanged(float temperature);
  void notifyRelHumidityChanged(float relHumidity);

private: // forbidden default functions
  MyDHT_ProcessAdapter& operator = (const MyDHT_ProcessAdapter& src); // assignment operator
  MyDHT_ProcessAdapter(const MyDHT_ProcessAdapter& src);              // copy constructor
};

#endif /* LIB_ADAPTERS_MYTEMPERATUREHUMIDITYADAPTER_H_ */
