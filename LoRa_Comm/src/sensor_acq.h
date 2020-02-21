/*
 * sensor_acq.h
 *
 *  Created on: 17 feb. 2020
 *      Author: villa
 */

#ifndef SENSOR_ACQ_H_
#define SENSOR_ACQ_H_

#include "main.h"
#include "ring_buf.h"


device_buf_t req_sensor_data(int count);
t_sensor sensor_data1();
t_sensor sensor_data2();
t_sensor sensor_data3();

#endif /* SENSOR_ACQ_H_ */
