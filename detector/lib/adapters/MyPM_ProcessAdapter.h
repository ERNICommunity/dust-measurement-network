/*
 * MyPmAdapter.h
 *
 *  Created on: 29.08.2017
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYPM_PROCESSADAPTER_H_
#define LIB_ADAPTERS_MYPM_PROCESSADAPTER_H_

#include <PM_Process.h>

class MyPM_ProcessAdapter : public PM_ProcessAdapter
{
public:
  MyPM_ProcessAdapter();
  virtual ~MyPM_ProcessAdapter();

  void notifyPmChanged(float pm10, float pm25);

private: // forbidden default functions
  MyPM_ProcessAdapter& operator = (const MyPM_ProcessAdapter& src); // assignment operator
  MyPM_ProcessAdapter(const MyPM_ProcessAdapter& src);              // copy constructor
};

#endif /* LIB_ADAPTERS_MYPM_PROCESSADAPTER_H_ */
