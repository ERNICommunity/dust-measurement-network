#include <LoraWanDataHandler.h>
#include "../Protobuf/dustMeasurement.pb.h"
#include <SerialCommand.h>

LoraWanDataHandler::LoraWanDataHandler(LoraWanPriorityQueue* a_LoraWanInterface):
m_LoraWanInterface(a_LoraWanInterface)
{

}

bool LoraWanDataHandler::encode(const void* a_Message, const pb_field_t* a_FieldType, uint8_t a_RetRawData[128], uint64_t* a_RetMessageLength)
{
   bool status = false;
  
  size_t messageLength;


  /* Create a stream that will write to our buffer. */
  pb_ostream_t stream = pb_ostream_from_buffer(a_RetRawData, 128);

  /* Now we are ready to encode the message! */
  status = pb_encode(&stream, a_FieldType, a_Message);
  messageLength = stream.bytes_written;

  /* Then just check for any errors.. */
  if (!status)
  {
    //TODO ENCODE THROW
   Serial.println("Error in Encoding");
  }
  else{
    *a_RetMessageLength = static_cast<uint64_t>(messageLength);
  }

  return status;
}