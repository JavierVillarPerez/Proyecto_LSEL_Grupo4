/*
 * fsm_sensor.c
 *
 *  Created on: 9 mar. 2020
 *      Author: villa
 */

#include "fsm_sensor.h"
#include "fsm.h"

void fsm_sensor_init(fsm_trans_t* sensor_fsm, sensor_t* sensor, t_bool active, t_bool supply, int data, int period, int setup, t_bool alarma, int threshold_H, int threshold_L, int average)
{
	sensor->fsm = fsm_new(sensor_fsm);
	sensor->active = active;
	sensor->supply = supply;
	sensor->data = data;
	sensor->period = period;
	sensor->setup = setup;
	sensor->alarma = alarma;
	sensor->threshold_H = threshold_H;
	sensor->threshold_L = threshold_L;
}
