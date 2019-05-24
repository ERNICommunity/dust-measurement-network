/*
 * MyMeasurementDataHandlerAdapter.h
 *
 *  Created on: 21.03.2019
 *      Author: nid
 */

#ifndef LIB_ADAPTERS_MYMEASUREMENFACADEADAPTER_H_
#define LIB_ADAPTERS_MYMEASUREMENFACADEADAPTER_H_

#include <IMeasurementFacadeAdapter.h>

class MeasurementFacade;
class PM_Process;
class DHT_Process;

class MyMeasuremenFacadeAdapter: public IMeasurementFacadeAdapter
{
public:
  MyMeasuremenFacadeAdapter(MeasurementFacade* measurementFacade, PM_Process* pmProcess, DHT_Process* dhtProcess);
  virtual ~MyMeasuremenFacadeAdapter();

  void fetchMeasuremenData();

private:
  MeasurementFacade* m_measurementFacade;
  PM_Process* m_pmProcess;
  DHT_Process* m_dhtProcess;
};

#endif /* LIB_ADAPTERS_MYMEASUREMENFACADEADAPTER_H_ */
