/*
 * LoRaWanDbgCmd.h
 *
 *  Created on: 12.01.2019
 *      Author: nid
 */

#ifndef LIB_LORAWAN_LORAWANDBGCMD_H_
#define LIB_LORAWAN_LORAWANDBGCMD_H_

#include <DbgCliCommand.h>

class LoRaWanDriver;

//-----------------------------------------------------------------------------

class LoRaWanDbgCmd_Configure : public DbgCli_Command
{
public:
  LoRaWanDbgCmd_Configure(LoRaWanDriver* loRaWanDriver);
  virtual ~LoRaWanDbgCmd_Configure();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();

private:
  LoRaWanDriver* m_loRaWanDriver;

private: // forbidden default functions
  LoRaWanDbgCmd_Configure& operator = (const LoRaWanDbgCmd_Configure& src); // assignment operator
  LoRaWanDbgCmd_Configure(const LoRaWanDbgCmd_Configure& src);              // copy constructor
  LoRaWanDbgCmd_Configure();                                                // default constructor
};

//-----------------------------------------------------------------------------

class LoRaWanDbgCmd_SingleChannel : public DbgCli_Command
{
public:
  LoRaWanDbgCmd_SingleChannel(LoRaWanDriver* loRaWanDriver);
  virtual ~LoRaWanDbgCmd_SingleChannel();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);

private:
  void printUsage();

private:
  LoRaWanDriver* m_loRaWanDriver;

private: // forbidden default functions
  LoRaWanDbgCmd_SingleChannel& operator = (const LoRaWanDbgCmd_SingleChannel& src); // assignment operator
  LoRaWanDbgCmd_SingleChannel(const LoRaWanDbgCmd_SingleChannel& src);              // copy constructor
  LoRaWanDbgCmd_SingleChannel();                                                    // default constructor
};

//-----------------------------------------------------------------------------

#endif /* LIB_LORAWAN_LORAWANDBGCMD_H_ */
