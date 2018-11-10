/*
 * Assets.h
 *
 *  Created on: 25.08.2018
 *      Author: kath
 */

#ifndef SRC_ASSETS_H_
#define SRC_ASSETS_H_

class DbgTrace_Port;
class DbgCli_Topic;
class DbgCli_Command;
class IAssetsDeviceSerialNrAdapter;

class Assets
{
public:
  Assets(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter = 0);
  virtual ~Assets();

  DbgCli_Topic* getCliTopicAssetsDeviceSerial() { return m_dbgCliTopicAssetsDeviceSerial; }

  void setDeviceSerialNrAdapter(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter);
  IAssetsDeviceSerialNrAdapter* getDeviceSerialNrAdapter();
  unsigned long int getDeviceSerialNr();
  void setDeviceSerialNr(unsigned long int deviceSerialNr);

private:
  DbgTrace_Port* m_trPort;
  DbgCli_Topic* m_dbgCliTopicAssets;
  DbgCli_Topic* m_dbgCliTopicAssetsDeviceSerial;
  DbgCli_Command* m_dbgCliSetDeviceSerialCmd;
  DbgCli_Command* m_dbgCliGetDeviceSerialCmd;
  unsigned long int m_deviceSerialNr;						          /// Device HW Serial Number (0..n)
  IAssetsDeviceSerialNrAdapter* m_deviceSerialNrAdapter;	/// Device Serial Nr. accessor adapter

private: // forbidden default functions
  Assets& operator = (const Assets& src); // assignment operator
  Assets(const Assets& src);              // copy constructor
  Assets();                               // default constructor
};

#endif /* SRC_ASSETS_H_ */
