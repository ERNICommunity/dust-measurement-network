/*
 * IMeasurementDataAdapter.h
 *
 *  Created on: 21.03.2019
 *      Author: nid
 */

#ifndef LIB_LORADATAHANDLER_IMEASUREMENTFACADEADAPTER_H_
#define LIB_LORADATAHANDLER_IMEASUREMENTFACADEADAPTER_H_

class IMeasurementFacadeAdapter
{
public:
  virtual ~IMeasurementFacadeAdapter() { }

  virtual void fetchMeasuremenData() = 0;

protected:
  IMeasurementFacadeAdapter() { }

private: // forbidden default functions
  IMeasurementFacadeAdapter& operator = (const IMeasurementFacadeAdapter& src); // assignment operator
  IMeasurementFacadeAdapter(const IMeasurementFacadeAdapter& src);              // copy constructor
};



#endif /* LIB_LORADATAHANDLER_IMEASUREMENTFACADEADAPTER_H_ */
