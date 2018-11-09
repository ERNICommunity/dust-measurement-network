/*
 * IAssetsDeviceSerialNrAdapter.h
 *
 *  Created on: 06.11.2018
 *      Author: nid
 */

#ifndef LIB_ASSETS_IASSETSDEVICESERIALNRADAPTER_H_
#define LIB_ASSETS_IASSETSDEVICESERIALNRADAPTER_H_

class IAssetsDeviceSerialNrAdapter
{
protected:
  IAssetsDeviceSerialNrAdapter() { }

public:
  virtual ~IAssetsDeviceSerialNrAdapter() { }

  virtual unsigned long int getDeviceSerialNr() = 0;

private: // forbidden default functions
  IAssetsDeviceSerialNrAdapter& operator = (const IAssetsDeviceSerialNrAdapter& src); // assignment operator
  IAssetsDeviceSerialNrAdapter(const IAssetsDeviceSerialNrAdapter& src);              // copy constructor
};

#endif /* LIB_ASSETS_IASSETSDEVICESERIALNRADAPTER_H_ */
