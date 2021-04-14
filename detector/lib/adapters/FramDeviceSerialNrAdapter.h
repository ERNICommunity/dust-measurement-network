/*
 * FramDeviceSerialNrAdapter.h
 *
 *  Created on: 13.08.2020
 *      Author: frda
 */

#ifndef LIB_ADAPTERS_FRAMDEVICESERIALNRADAPTER_H_
#define LIB_ADAPTERS_FRAMDEVICESERIALNRADAPTER_H_

#include <IAssetsDeviceSerialNrAdapter.h>

class FramDeviceSerialNrAdapter: public IAssetsDeviceSerialNrAdapter
{
public:
  FramDeviceSerialNrAdapter();
  virtual ~FramDeviceSerialNrAdapter();

  virtual unsigned long int getDeviceSerialNr();

private:

};

#endif /* LIB_ADAPTERS_FRAMDEVICESERIALNRADAPTER_H_ */