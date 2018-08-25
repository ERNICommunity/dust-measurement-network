/*
 * Assets.h
 *
 *  Created on: 25.08.2018
 *      Author: kath
 */

#ifndef SRC_ASSETS_H_
#define SRC_ASSETS_H_

class Adafruit_FRAM_I2C;
class DbgTrace_Port;


class Assets
{
public:
  Assets(Adafruit_FRAM_I2C* fram);
  virtual ~Assets();

  static const int MAX_ID_LENGTH = 64;
  void setDeviceId(const char* id);
  const char* getDeviceId();

private:
  char m_id[MAX_ID_LENGTH + 1]; // one more to ensure we have termination!
  Adafruit_FRAM_I2C* m_fram;
  DbgTrace_Port* m_trPort;

private: // forbidden default functions
  Assets& operator = (const Assets& src); // assignment operator
  Assets(const Assets& src);              // copy constructor
  Assets();                               // default constructor
};

#endif /* SRC_ASSETS_H_ */
