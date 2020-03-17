/*
 * fsm_sensor.h
 *
 *  Created on: Mar 10, 2020
 *      Author: villa
 */

#ifndef INC_FSM_SENSOR_H_
#define INC_FSM_SENSOR_H_

#include "fsm.h"
#include "main.h"
//#include "ring_buf.h"

typedef struct{
	fsm_t* fsm;
	t_bool active;
	t_bool supply;
	int data;
	int period;
	int setup;
	t_bool alarma;
	int threshold_H;
	int threshold_L;
	int average;
}sensor_t;


void fsm_sensor_init(fsm_trans_t* sensor_fsm, sensor_t* sensor, t_bool active, t_bool supply, int data, int period, int setup, t_bool alarma, int threshold_H, int threshold_L, int average);


#endif /* INC_FSM_SENSOR_H_ */
