#ifndef _LoraDataHandler_MeasurementFacade_h_
#define _LoraDataHandler_MeasurementFacade_h_

#include <inttypes.h>
#include <LoraWanPriorityQueue.h>
#include <LoraWanDataHandler.h>


class IMesurementAdapter;
class LoraWanPriorityQueue;
class IMeasurementDataAdapter;
class PM_Process;
class DHT_Process;

class MeasurementFacade : public LoraWanDataHandler
{
  public:
    MeasurementFacade(LoraWanPriorityQueue* a_LoraWanInterface, IMeasurementDataAdapter* adapter = 0);

    void attachAdapter(IMeasurementDataAdapter* adapter);
    IMeasurementDataAdapter* adapter();

    void setNewMeasurementData(float a_ParticularMatter2_5, float a_ParticularMatter10, float a_Temperature, float a_Humidity);

    void updateMeasurementData();

  private:
    IMeasurementDataAdapter* m_adapter;
};
#endif
