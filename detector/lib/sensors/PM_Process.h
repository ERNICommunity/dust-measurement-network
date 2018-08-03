/*
 * SDS011.h
 *
 *  Created on: 27.08.2017
 *      Author: scan
 */

#ifndef LIB_SENSORS_PM_PROCESS_H_
#define LIB_SENSORS_PM_PROCESS_H_

#include <Arduino.h>

class DbgTrace_Port;

class PM_ProcessAdapter
{
public:
  virtual ~PM_ProcessAdapter() { }
  virtual void notifyPmChanged(float pm10, float pm25) = 0;
};

class PM_Process
{
public:
  PM_Process(HardwareSerial* serial, PM_ProcessAdapter* pmAdapter = 0);
  virtual ~PM_Process();

public:
  /**
   * print all child nodes to console output
   */
  void pollSerialData();

  void init(uint32_t baudrate)
  {
    m_serial->begin(baudrate);
  }

  float getPm10Average();
  float getPm25Average();

private:
  bool validateChecksum();
  void storeToBuffer(float pm10, float pm25);

private:
  HardwareSerial* m_serial;
  uint8_t m_readIndex;
  uint8_t m_data[10];

  static const uint8_t BUFFER_SIZE = 60;  // Buffer for average calculation
  float m_pm10[BUFFER_SIZE];
  float m_pm25[BUFFER_SIZE];

  PM_ProcessAdapter* m_pmProcessAdapter;
  DbgTrace_Port* m_trPort;
};

#endif /* LIB_SENSORS_PM_PROCESS_H_ */
