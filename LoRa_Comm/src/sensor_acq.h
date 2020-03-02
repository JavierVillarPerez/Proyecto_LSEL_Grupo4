/*
 ============================================================================
 Name        : sensor_acq.h
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Sensor Acquisition, Ansi-style
 ============================================================================
 */

#ifndef SENSOR_ACQ_H_
#define SENSOR_ACQ_H_

#include "main.h"
#include "ring_buf.h"


device_buf_t req_sensor_data(int count);
t_sensor sensor_data1(int increment);
t_sensor sensor_data2(int increment);
t_sensor sensor_data3(int increment);

#endif /* SENSOR_ACQ_H_ */
