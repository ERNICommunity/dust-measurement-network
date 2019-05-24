#ifndef _LoraDataHandler_LoraWanDataHandler_h_
#define _LoraDataHandler_LoraWanDataHandler_h_
#include <inttypes.h>
#include "stdlib.h"
#include <pb_encode.h>

class LoraWanPriorityQueue;

/**
 * LoRaWan Data Handler.
 *
 */
class LoraWanDataHandler
{
  public:
    LoraWanDataHandler(LoraWanPriorityQueue* a_LoraWanInterface);

  protected:
    bool encode(const void* a_Message, const pb_field_t* a_FieldType, uint8_t a_RetRawData[128], uint64_t* a_RetMessageLength);

    LoraWanPriorityQueue* loRaWanPriorityQueue();

  private:
    LoraWanPriorityQueue* m_priorityQueue;
};
#endif
