/*
 * MyBuiltinLedIndicatorAdapter.h
 *
 *  Created on: 04.11.2019
 *      Author: nid
 */

#ifndef LIB_ADAPTER_MYBUILTINLEDINDICATORADAPTER_H_
#define LIB_ADAPTER_MYBUILTINLEDINDICATORADAPTER_H_

#include <Indicator.h>

class MyBuiltinLedIndicatorAdapter: public AIndicatorAdapter
{
public:
  MyBuiltinLedIndicatorAdapter();
  virtual ~MyBuiltinLedIndicatorAdapter();
  void notifyStatusChange(bool status);

private:
  void setLed(bool isOn);

private:  // forbidden default functions
  MyBuiltinLedIndicatorAdapter(const MyBuiltinLedIndicatorAdapter &other) = delete;
  MyBuiltinLedIndicatorAdapter(MyBuiltinLedIndicatorAdapter &&other) = delete;
};

#endif /* LIB_ADAPTER_MYBUILTINLEDINDICATORADAPTER_H_ */
