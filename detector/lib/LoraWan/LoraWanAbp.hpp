#ifndef _LoraWan_LoraWanAbp_h_
#define _LoraWan_LoraWanAbp_h_
#include <Arduino.h>

#ifdef __cplusplus
   extern "C"
    {
#endif

#include <lmic.h>

void os_getArtEui(u1_t *buf);
void os_getDevEui(u1_t *buf);
void os_getDevKey(u1_t *buf);


void do_send();

void onEvent(ev_t ev);

void configuration();

void loop_once();
#ifdef __cplusplus
 }
#endif

#include "LoraWanAdapter.hpp"

class LoraWanAbp : public LoraWanAdapter
{
    public:
        LoraWanAbp();
        void loopOnce();
        bool isReadyToRead();
        uint64_t readData(uint8_t* const a_Data, uint64_t a_MaxSizeOfBuffer);
        void setPeriodicMessageData(uint8_t* a_Data, uint64_t a_SizeOfData);
        uint64_t getSentCounterPeriodicMessage();
    private:
        bool m_ConnectionIsConfigured;
        void configure();
};
#endif
