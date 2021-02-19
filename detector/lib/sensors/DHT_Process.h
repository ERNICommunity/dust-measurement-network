/*
 * TemperatureHumidity.h
 *
 *  Created on: 27.08.2017
 *      Author: nid
 */

#ifndef SRC_TEMPERATUREHUMIDITY_H_
#define SRC_TEMPERATUREHUMIDITY_H_

class DHT_Unified;
class SpinTimer;

//-----------------------------------------------------------------------------

class DHT_ProcessAdapter
{
public:
  virtual void notifyTemperatureChanged(float temperature) = 0;
  virtual void notifyRelHumidityChanged(float relHumidity) = 0;
  virtual ~DHT_ProcessAdapter() { }
};

//-----------------------------------------------------------------------------

class DHT_Process
{
  friend class DhtPollTimerAdapter;

public:
  DHT_Process(DHT_ProcessAdapter* dhtProcessAdapter = 0);
  virtual ~DHT_Process();

  float getRelHumidity();
  float getTemperature();

protected:
  void setRelHumidity(float relHumidity);
  void setTemperature(float temperature);

private:
  DHT_Unified* m_dht;
  unsigned long int m_delayMs;
  SpinTimer* m_dhtPollTimer;
  float m_relHumidity;
  float m_temperature;
  DHT_ProcessAdapter* m_dhtProcessAdapter;

private: // forbidden default functions
  DHT_Process& operator = (const DHT_Process& src); // assignment operator
  DHT_Process(const DHT_Process& src);              // copy constructor
};

#endif /* SRC_TEMPERATUREHUMIDITY_H_ */
