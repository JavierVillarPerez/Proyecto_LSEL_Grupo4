/*
 * fsm_sensor.h
 *
 *  Created on: Mar 28, 2020
 *      Author: villa
 */

#ifndef INC_FSM_SENSOR_H_
#define INC_FSM_SENSOR_H_

#include "main.h"
#include "fsm.h"
#include "stm32f4xx_hal_gpio.h"
//#include "ring_buf.h"

typedef struct {
	/*SENSOR CONTROL*/
	t_bool active;    	//active the sensor
	uint16_t data_recovered; // recovered data
	uint16_t data_average;
	t_bool alarm;		// warning signal
	t_bool error;		// sensor´s error
	long setup_timer;
	long sleep_timer;
	long warning_timer;
	long warning_count;
	long measure_timer;
	uint8_t measure_count;

	/*SENSOR PARAMS*/
	uint16_t ID;
	uint16_t supply_Pin;		//supply´s pin to the sensor
	uint16_t threshold_L;
	uint16_t threshold_H;
	uint16_t threshold_Max;	//Max range of data
	uint16_t setup_period;
	uint16_t sleep_period;
	uint16_t warning_period;
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
int timer_warning (fsm_t* this);
int timer_measure (fsm_t* this);
int contador (fsm_t* this);
int warning_limit (fsm_t* this);
void measuring (fsm_t* this);
void process_data (fsm_t* this);
void alert (fsm_t* this);
void sleep (fsm_t* this);
void setting_up (fsm_t* this);
void save_data (fsm_t* this);

void sensor_initialization(sensor_t* sensor, uint16_t ID, uint16_t supply_Pin, uint16_t threshold_L, uint16_t threshold_H, uint16_t threshold_Max, uint16_t setup_period, uint16_t warning_period, uint16_t sleep_period, uint16_t measure_period, uint16_t measure_average);


#endif /* INC_FSM_SENSOR_H_ */
