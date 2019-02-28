/*
 * LoRaWanRxDataToStatusLedAdapter.cpp
 *
 *  Created on: 19.02.2019
 *      Author: nid
 */

#include <ToggleButton.h>
#include <LoRaWanDriver.h>
#include <LoRaWanRxDataToStatusLedAdapter.h>


LoRaWanRxDataToStatusLedAdapter::LoRaWanRxDataToStatusLedAdapter(ToggleButton* statusLed, LoRaWanDriver* loRaWanDriver)
: m_statusLed(statusLed)
, m_loRaWanDriver(loRaWanDriver)
{ }

LoRaWanRxDataToStatusLedAdapter::~LoRaWanRxDataToStatusLedAdapter()
{ }

void LoRaWanRxDataToStatusLedAdapter::messageReceived(unsigned char* payload, unsigned int length)
{
  if ((0 != m_loRaWanDriver) && (0 != m_statusLed))
  {
    if (length >= 2)
    {
      if (payload[0] == 0)
      {
        m_loRaWanDriver->setIsLoRaWanHeartBeat(false);
        m_statusLed->setIsActive(payload[1] != 0);
      }
    }

    if (payload[0] > 0)
    {
      m_loRaWanDriver->setIsLoRaWanHeartBeat(true);
    }
  }
}
