 /*
  * Assets.h
  *
  *  Created on: 25.08.2018
  *      Author: kath
  */

#include<string.h>
#include "Adafruit_FRAM_I2C.h" // PIO Lib: 658 -> https://platformio.org/lib/show/658/Adafruit%20FRAM%20I2C
#include "Assets.h"

Assets::Assets()
: m_fram(new Adafruit_FRAM_I2C())
{
  memset(m_id, 0, MAX_ID_LENGTH + 1); // one more to ensure we have termination!
  bool state = m_fram->begin();
  if (!state)
  {
      // ToDo: Add error trace!
    delete m_fram;
    m_fram = 0;
    return;
  }
  char nibble;
  int index = 0;
  while ((nibble = m_fram->read8(index++)) != '\0' && index < MAX_ID_LENGTH)
  {
    m_id[index] = nibble;
  }
  // ToDo: We have to deal with un-initialized data! and warn appropriately
}

Assets::~Assets()
{
  if (m_fram == 0)
  {
    return;
  }
  delete m_fram;
  m_fram = 0;
}

void Assets::setDeviceId(const char* id){
  strncpy(m_id, id, MAX_ID_LENGTH);
  if (m_fram == 0)
  {
    // ToDo: Add error trace!
    return;
  }
  char nibble;
  int index = 0;
  while ((nibble = id[index++]) != '\0' && index < MAX_ID_LENGTH)
  {
    m_fram->write8(index, nibble);
  }
  // ToDo: emmit eror when index == MAX_ID_LENGTH!
  m_fram->write8(index, (uint8_t)'\0'); // and always terminate!
}

const char* Assets::getDeviceId()
{
  return m_id;
}
