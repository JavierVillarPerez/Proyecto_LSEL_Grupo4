/*
 * fsm_sensor.h
 *
 * Description: fsm_sensor.c library.
 *
 *  Created on: 9 mar. 2020
 *      Author: Javier Villar
 */

#ifndef INC_FSM_SENSOR_H_
#define INC_FSM_SENSOR_H_

#include "main.h"
#include "fsm.h"
#include "ring_buf.h"
#include "stm32f4xx_hal_gpio.h"
//#include "ring_buf.h"

typedef struct {
	/*SENSOR CONTROL: this params must be initialized to 0/FALSE (except active which must be initialized to TRUE)*/
	t_bool active;    		 /*Sensor can be activated or desactivated by software*/
	uint16_t data_recovered; /*Measured data*/
	uint16_t data_average;	 /*Measured data after average*/
	uint8_t alarm;			 /*Warning signal*/
	uint8_t  error;		     /*Sensor error signal*/
	t_bool measuring;		 /*Measuring flag*/
	t_bool sleeping;		 /*Sleeping flag*/
	long setup_timer;
	long sleep_timer;
	long warning_count;
	long measure_timer;
	uint8_t measure_count;

	/*SENSOR PARAMS: Params defined in macros in the main.c*/
	uint16_t Device_ID;		/*ID for device: Identify the water source*/
	uint8_t  Sensor_ID;		/*ID for sensor: Identify the sensor (ph, turbidity...)*/
	uint16_t supply_Pin;	/*GPIO to turn ON/OFF the sensor*/
	uint16_t threshold_L;
	uint16_t threshold_H;
	uint16_t threshold_Max;	/*Max range of data, if sensor reach this value => ERROR FLAG*/
	uint16_t setup_period;
	uint16_t sleep_period;
	uint16_t measure_period;
	uint16_t measure_average;
}sensor_t;

typedef struct {
    fsm_t fsm;
    sensor_t* param;
}fsm_sensor_t;

fsm_sensor_t* fsm_sensor_new (sensor_t* param);
void fsm_sensor_init(fsm_sensor_t* sensor_fsm, sensor_t* param);
int data_saved(fsm_t* this);
int error (fsm_t* this);
int no_error (fsm_t* this);
int timer_sleep (fsm_t* this);
int timer_setup (fsm_t* this);
int warned (fsm_t* this);
int timer_measure (fsm_t* this);
int average_counter (fsm_t* this);
int warning_limit (fsm_t* this);
void measuring (fsm_t* this);
void process_data (fsm_t* this);
void alert (fsm_t* this);
void sleep (fsm_t* this);
void setting_up (fsm_t* this);
void save_data (fsm_t* this);

void sensor_initialization(sensor_t* sensor, uint16_t Device_ID, uint8_t Sensor_ID, uint16_t supply_Pin, uint16_t threshold_L, uint16_t threshold_H, uint16_t threshold_Max, uint16_t setup_period, uint16_t sleep_period, uint16_t measure_period, uint16_t measure_average);


void save_new_data(sensor_buf_t data);


#endif /* INC_FSM_SENSOR_H_ */
