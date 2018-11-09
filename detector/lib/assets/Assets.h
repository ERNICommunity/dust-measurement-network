/*
 * Assets.h
 *
 *  Created on: 25.08.2018
 *      Author: kath
 */

#ifndef SRC_ASSETS_H_
#define SRC_ASSETS_H_

class DbgTrace_Port;
class IAssetsDeviceSerialNrAdapter;

class Assets
{
public:
  Assets(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter = 0);
  virtual ~Assets();

  void setDeviceSerialNrAdapter(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter);
  IAssetsDeviceSerialNrAdapter* getDeviceSerialNrAdapter();
  unsigned long int getDeviceSerialNr();

private:
  DbgTrace_Port* m_trPort;
  unsigned long int m_deviceSerialNr;						/// Device HW Serial Number (0..n)
  IAssetsDeviceSerialNrAdapter* m_deviceSerialNrAdapter;	/// Device Serial Nr. accessor adapter

private: // forbidden default functions
  Assets& operator = (const Assets& src); // assignment operator
  Assets(const Assets& src);              // copy constructor
  Assets();                               // default constructor
};

#endif /* SRC_ASSETS_H_ */
