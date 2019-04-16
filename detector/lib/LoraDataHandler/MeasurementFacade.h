#ifndef _LoraDataHandler_MeasurementFacade_h_
#define _LoraDataHandler_MeasurementFacade_h_

#include <inttypes.h>
#include <LoraWanPriorityQueue.h>
#include <LoraWanDataHandler.h>


class IMesurementAdapter;
class LoraWanPriorityQueue;
class IMeasurementFacadeAdapter;
class PM_Process;
class DHT_Process;

class MeasurementFacade : public LoraWanDataHandler
{
  public:
    MeasurementFacade(LoraWanPriorityQueue* a_LoraWanInterface, IMeasurementFacadeAdapter* adapter = 0);

    void attachAdapter(IMeasurementFacadeAdapter* adapter);
    IMeasurementFacadeAdapter* adapter();

    void setNewMeasurementData(float a_ParticularMatter2_5, float a_ParticularMatter10, float a_Temperature, float a_Humidity);

    void updateMeasurementData();

  private:
    IMeasurementFacadeAdapter* m_adapter;
};
#endif
