/*
 * AssetsDbgCmd.h
 *
 *  Created on: 09.11.2018
 *      Author: nid
 */

#ifndef LIB_ASSETS_ASSETSDBGCMD_H_
#define LIB_ASSETS_ASSETSDBGCMD_H_

#include <DbgCliCommand.h>

class Assets;

class AssetsDbgCmd_SetDeviceSerial : public DbgCli_Command
{
public:
  AssetsDbgCmd_SetDeviceSerial(Assets* assets);
  virtual ~AssetsDbgCmd_SetDeviceSerial();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();

private:
  Assets* m_assets;

private: // forbidden default functions
  AssetsDbgCmd_SetDeviceSerial& operator = (const AssetsDbgCmd_SetDeviceSerial& src); // assignment operator
  AssetsDbgCmd_SetDeviceSerial(const AssetsDbgCmd_SetDeviceSerial& src);              // copy constructor
  AssetsDbgCmd_SetDeviceSerial();                                                     // default constructor
};

//-----------------------------------------------------------------------------

class AssetsDbgCmd_GetDeviceSerial : public DbgCli_Command
{
public:
  AssetsDbgCmd_GetDeviceSerial(Assets* assets);
  virtual ~AssetsDbgCmd_GetDeviceSerial();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();

private:
  Assets* m_assets;

private: // forbidden default functions
  AssetsDbgCmd_GetDeviceSerial& operator = (const AssetsDbgCmd_GetDeviceSerial& src); // assignment operator
  AssetsDbgCmd_GetDeviceSerial(const AssetsDbgCmd_GetDeviceSerial& src);              // copy constructor
  AssetsDbgCmd_GetDeviceSerial();                                                     // default constructor
};

//-----------------------------------------------------------------------------

class AssetsDbgCmd_GetLoRaKeys : public DbgCli_Command
{
public:
  AssetsDbgCmd_GetLoRaKeys(Assets* assets);
  virtual ~AssetsDbgCmd_GetLoRaKeys();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();

private:
  Assets* m_assets;

private: // forbidden default functions
  AssetsDbgCmd_GetLoRaKeys& operator = (const AssetsDbgCmd_GetLoRaKeys& src);       // assignment operator
  AssetsDbgCmd_GetLoRaKeys(const AssetsDbgCmd_GetLoRaKeys& src);                    // copy constructor
  AssetsDbgCmd_GetLoRaKeys();                                                       // default constructor
};

#endif /* LIB_ASSETS_ASSETSDBGCMD_H_ */
