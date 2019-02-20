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
class IPersistentDataMemory;

class Assets
{
public:
  Assets(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter = 0, IPersistentDataMemory* persistentDataMemory = 0);
  virtual ~Assets();

  DbgCli_Topic* getCliTopicAssetsDeviceSerial() { return m_dbgCliTopicAssetsDeviceSerial; }
  DbgCli_Topic* getCliTopicAssetsLoRaKeys()     { return m_dbgCliTopicAssetsLoRaKeys; }

  void setDeviceSerialNrAdapter(IAssetsDeviceSerialNrAdapter* deviceSerialNrAdapter);
  IAssetsDeviceSerialNrAdapter* getDeviceSerialNrAdapter();

  void setPersistentDataMemory(IPersistentDataMemory* persistentDataMemory);
  IPersistentDataMemory* getPersistentDataMemory();

  unsigned long int getDeviceSerialNr();
  void setDeviceSerialNr(unsigned long int deviceSerialNr);

  DbgTrace_Port* trPort();

  /**
   * Read Device Id string.
   * @param deviceId OUT: Buffer for Device Id string to read from Persistent Data Memory.
   * @param len Buffer size for Device Id
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getDeviceId( char* deviceId, unsigned int len);

  /**
   * Read Device Address string.
   * @param devAddr OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Device Address
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getDevAddr(  char* devAddr,  unsigned int len);

  /**
   * Read Network Security Key string.
   * @param Network Security Key OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Network Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getNwkSKey( char* nwkSKey,  unsigned int len);

  /**
   * Read Application Security Key string.
   * @param Application Security Key OUT: Buffer for Device Address string to read from Persistent Data Memory.
   * @param len Buffer size for Application Security Key
   * @return Number of characters read (including '\0' terminator character)
   */
  unsigned int getAppSKey(  char* appSKey,  unsigned int len);

private:
  DbgTrace_Port* m_trPort;
  DbgCli_Topic* m_dbgCliTopicAssets;
  DbgCli_Topic* m_dbgCliTopicAssetsDeviceSerial;
  DbgCli_Topic* m_dbgCliTopicAssetsLoRaKeys;
  DbgCli_Command* m_dbgCliSetDeviceSerialCmd;
  DbgCli_Command* m_dbgCliGetDeviceSerialCmd;
  DbgCli_Command* m_dbgCliGetLoRaKeysCmd;
  unsigned long int m_deviceSerialNr;						          /// Device HW Serial Number (0..n)
  IAssetsDeviceSerialNrAdapter* m_deviceSerialNrAdapter;	/// Device Serial Nr. accessor adapter
  IPersistentDataMemory* m_persistentDataMemory;

private: // forbidden default functions
  Assets& operator = (const Assets& src); // assignment operator
  Assets(const Assets& src);              // copy constructor
  Assets();                               // default constructor
};

#endif /* SRC_ASSETS_H_ */
