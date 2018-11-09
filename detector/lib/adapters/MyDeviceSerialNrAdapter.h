/*
 * MyDeviceSerialNrAdapter.h
 *
 *  Created on: 06.11.2018
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYDEVICESERIALNRADAPTER_H_
#define LIB_ADAPTERS_MYDEVICESERIALNRADAPTER_H_

#include <IAssetsDeviceSerialNrAdapter.h>

class MyDeviceSerialNrAdapter: public IAssetsDeviceSerialNrAdapter
{
public:
  MyDeviceSerialNrAdapter();
  virtual ~MyDeviceSerialNrAdapter();

  virtual unsigned long int getDeviceSerialNr();

private:
  const static int s_serialNr_bit0;
  const static int s_serialNr_bit1;
  const static int s_serialNr_bit2;
  const static int s_serialNr_bit3;
  const static int s_serialNr_bit4;
};

#endif /* LIB_ADAPTERS_MYDEVICESERIALNRADAPTER_H_ */
