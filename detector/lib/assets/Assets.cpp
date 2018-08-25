 /*
  * Assets.h
  *
  *  Created on: 25.08.2018
  *      Author: kath
  */

#include<string.h>
#include "Adafruit_FRAM_I2C.h" // PIO Lib: 658 -> https://platformio.org/lib/show/658/Adafruit%20FRAM%20I2C
#include "Assets.h"
#include <DbgTracePort.h>
#include <DbgTraceLevel.h>

const int Assets::MAX_ID_LENGTH = 64;

Assets::Assets(Adafruit_FRAM_I2C* fram)
: m_id(new char(MAX_ID_LENGTH + 1)), // one more to ensure we have termination!
  m_fram(fram),
  m_trPort(new DbgTrace_Port("fram", DbgTrace_Level::info))
{
  memset(m_id, 0, MAX_ID_LENGTH + 1); // one more to ensure we have termination!
  if (m_fram == 0)
  {
    TR_PRINT_STR(m_trPort, DbgTrace_Level::error, "FRAM is NULL -> no FRAM available!");
    return;
  }
  char nibble;
  int index = 0;
  while (index < MAX_ID_LENGTH && (nibble = m_fram->read8(index)) != '\0')
  {
    m_id[index++] = nibble;
  }
  if (index >= MAX_ID_LENGTH)
  {
    TR_PRINT_STR(m_trPort, DbgTrace_Level::warning, "FRAM may not have been initialized properly.");
  }
}

Assets::~Assets()
{
  delete m_id;
  m_id = 0;
}

void Assets::setDeviceId(const char* id)
{
  strncpy(m_id, id, MAX_ID_LENGTH);
  if (m_fram == 0)
  {
    return; // no error messages, as we reported missing fram in constructor.
  }
  char nibble;
  int index = 0;
  while (index < MAX_ID_LENGTH && (nibble = id[index]) != '\0')
  {
    m_fram->write8(index++, nibble);
  }
  if (index >= MAX_ID_LENGTH)
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::warning, "device id has been truncated to %s", m_id);
  }
  m_fram->write8(index, (uint8_t)'\0'); // and always terminate!
}

const char* Assets::getDeviceId()
{
  return m_id;
}
