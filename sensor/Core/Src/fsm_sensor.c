/*
 * fsm_sensor.c
 *
 *  Created on: Mar 28, 2020
 *      Author: villa
 */


/*
 * fsm_sensor.c
 *
 *  Created on: 9 mar. 2020
 *      Author: villa
 */

#include "fsm_sensor.h"
#include "ring_buf.h"
#include "LoRa_comm.h"

ADC_HandleTypeDef hadc1;

enum{
	  Measure,
	  Warning,
	  Process,
	  Saving,
	  Sleeping,
	  Setup
}sensor_state;

int data_saved(fsm_t* this){ return 1; }

int error (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	if (config->data_average <= 0 || config->data_average > config->threshold_Max)
		return 1;
	else return 0;
}

int no_error (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	if (config->data_average > 0 && config->data_average <= config->threshold_Max)
		return 1;
	else return 0;
}

int timer_sleep (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	if(config->active && HAL_GetTick()>config->sleep_timer) return 1;
	else return 0;
}

int timer_setup (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return (HAL_GetTick()>config->setup_timer);
}

int timer_warning (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return (HAL_GetTick()>config->warning_timer);
}

int timer_measure (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return (HAL_GetTick()>config->measure_timer);
}

int contador (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	if(config->measure_count>=(config->measure_average))
		return 1;
	else return 0;
}

int warning_limit (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return(config->warning_count >= 2);
}

void measuring (fsm_t* this) {
	uint16_t data=0;
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;

	data = HAL_ADC_GetValue(&hadc1);

	config->data_recovered = config->data_recovered + data;
	config->measure_count = config->measure_count +1;
	config->measure_timer = HAL_GetTick()+ config->measure_period;

	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, SET);
}

void process_data (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->data_average = config->data_recovered/config->measure_count;
	HAL_ADC_Stop(&hadc1);
	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, 1);

	if (config->data_average <= config->threshold_H && config->data_average >= config->threshold_L)
	{
		config->alarm = FALSE;
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, RESET);
	}
	else
	{
		config->alarm = TRUE;
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);

	}

}
void alert (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->warning_count = config->warning_count +1;
	config->warning_timer = HAL_GetTick()+config->warning_period;
}

void sleep (fsm_t* this) { // led orange
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->sleep_timer = HAL_GetTick()+config->sleep_period;

	HAL_GPIO_WritePin(GPIOD, config->supply_Pin, RESET);

	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, RESET);

	if(config->warning_count >= 2)
	{
		config->error = TRUE;
		HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, SET);
		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, SET);
		HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, RESET);
	}
}

void setting_up (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->measure_count = 0;
	config->warning_count = 0;
	config->data_recovered = 0;
	config->data_average = 0;
	config->error = FALSE;
	config->setup_timer = HAL_GetTick() + config->setup_period;
	HAL_GPIO_WritePin(GPIOD, config->supply_Pin, SET);
	HAL_ADC_Start(&hadc1);

	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, SET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, RESET);
}

void save_data (fsm_t* this)
{
	sensor_buf_t data2save;

	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;

	data2save.ID = config->ID;
	data2save.alarm = config->alarm;
	data2save.error = config->error;
	data2save.measure = config->data_average;
	data2save.threshold_H = config->threshold_H;
	data2save.threshold_L = config->threshold_L;
//	data2save.timestamp =

	save_new_data(data2save);
}

fsm_trans_t trans_sensor[] = {
  { Sleeping, timer_sleep,   Setup,    setting_up},
  { Setup,    timer_setup,   Measure,  measuring},
  { Measure,  timer_measure, Measure,  measuring},
  { Measure,  contador,      Process,  process_data},
  { Process,  error,         Warning,  alert},
  { Process,  no_error,      Saving,   save_data},
  { Warning,  timer_warning, Measure,  measuring},
  { Warning,  warning_limit, Saving,   save_data},
  { Saving,   data_saved,    Sleeping, sleep},
  {-1, NULL, -1, NULL },
};

#ifdef USE_MALLOC
fsm_sensor_t* fsm_sensor_new (sensor_t* param) {
  fsm_param_t* fl = (fsm_param_t*) malloc (sizeof (fsm_param_t));
  fsm_param_init (fl, param);
  return fl;
}
#endif

void fsm_sensor_init (fsm_sensor_t* f, sensor_t* c) {
  fsm_init ((fsm_t*)f, trans_sensor);
  f->param = c;
}


void sensor_initialization(sensor_t* sensor, uint16_t ID, uint16_t supply_Pin, uint16_t threshold_L, uint16_t threshold_H, uint16_t threshold_Max, uint16_t setup_period, uint16_t warning_period, uint16_t sleep_period, uint16_t measure_period, uint16_t measure_average)
{
	/*SENSOR CONTROL*/
	sensor->active = TRUE;
	sensor->data_recovered = 0;
	sensor->data_average = 0;
	sensor->alarm = FALSE;
	sensor->error = FALSE;
	sensor->setup_timer = 0;
	sensor->sleep_timer = 0;
	sensor->warning_timer = 0;
	sensor->warning_count = 0;
	sensor->measure_timer = 0;
	sensor->measure_count = 0;

	/*SENSOR PARAMS*/
	sensor->ID = ID;
	sensor->supply_Pin = supply_Pin;
	sensor->threshold_L = threshold_L;
	sensor->threshold_H = threshold_H;
	sensor->threshold_Max = threshold_Max;
	sensor->setup_period = setup_period;
	sensor->sleep_period = sleep_period;
	sensor->warning_period = warning_period;
	sensor->measure_period = measure_period;
	sensor->measure_average = measure_average;

}
