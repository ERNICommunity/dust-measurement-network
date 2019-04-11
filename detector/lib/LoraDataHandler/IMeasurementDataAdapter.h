/*
 * IMeasurementDataAdapter.h
 *
 *  Created on: 21.03.2019
 *      Author: nid
 */

#ifndef LIB_LORADATAHANDLER_IMEASUREMENTDATAADAPTER_H_
#define LIB_LORADATAHANDLER_IMEASUREMENTDATAADAPTER_H_

class IMeasurementDataAdapter
{
public:
  virtual ~IMeasurementDataAdapter() { }

  virtual void fetchMeasuremenData() = 0;

protected:
  IMeasurementDataAdapter() { }

private: // forbidden default functions
  IMeasurementDataAdapter& operator = (const IMeasurementDataAdapter& src); // assignment operator
  IMeasurementDataAdapter(const IMeasurementDataAdapter& src);              // copy constructor
};



#endif /* LIB_LORADATAHANDLER_IMEASUREMENTDATAADAPTER_H_ */
