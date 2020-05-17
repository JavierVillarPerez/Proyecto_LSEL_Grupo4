/*
 * fsm_sensor.c
 *
 * Description: FSM for sensor control.
 * 				This FSM has been created in order to take measurements
 * 				from the ADC for different sensors with different
 * 				configurations.
 *
 *  Created on: 9 mar. 2020
 *      Author: Javier Villar
 */

#include "fsm_sensor.h"
#include "main.h"
#include "adc.h"

#define adc_setup_time 250 /*Time to ensure ADC switched on*/

uint8_t adc_timer;
t_bool  data_saved_flg;

enum{
	  Pre_setup,
	  Setup,
	  Measure,
	  Warning,
	  Process,
	  Saving,
	  Sleeping
}sensor_state;

int data_saved(fsm_t* this)
{
	return data_saved_flg;
}

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
	if(config->active && HAL_GetTick()>=config->sleep_timer) return 1;
	else return 0;
}

int timer_adc (fsm_t* this) {
	return (HAL_GetTick()>250); /*Timer to ensure ADC setup*/
}

int timer_setup (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return (HAL_GetTick()>config->setup_timer);
}

int warned (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	return (config->warning_count < 2);
}

int average_counter (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	if(config->measure_count>=(config->measure_average))
	return 1;
	else return 0;
}


int timer_measure (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;

	return (HAL_GetTick()>=(config->measure_timer));
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
}

void process_data (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->data_average = config->data_recovered/config->measure_average;
	config->measure_count = 0;
	config->data_recovered = 0;

	HAL_ADC_Stop(&hadc1);

	if (config->data_average <= config->threshold_H && config->data_average >= config->threshold_L)
	{
		config->alarm = 0;
	}
	else
	{
		config->alarm = 1;
	}

}
void alert (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->warning_count = config->warning_count +1;
}

void sleep (fsm_t* this) { // led orange
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->sleep_timer = HAL_GetTick()+config->sleep_period;

	HAL_GPIO_WritePin(GPIOD, config->supply_Pin, RESET);

	config->sleeping = TRUE;
	config->measuring = FALSE;

	data_saved_flg = FALSE;

	if(config->warning_count >= 2)
	{
		config->error = 1;
	}
	else
	{
		config->error = 0;
	}
}

void init_adc (fsm_t* this) {
	HAL_ADC_Stop(&hadc1);

	adc_timer = HAL_GetTick() + adc_setup_time;

}


void setting_up (fsm_t* this) {
	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;
	config->measure_count = 0;
	config->warning_count = 0;
	config->data_recovered = 0;
	config->data_average = 0;
	config->setup_timer = HAL_GetTick() + config->setup_period;
	HAL_GPIO_WritePin(GPIOD, config->supply_Pin, SET);

	HAL_ADC_Start(&hadc1);

	config->measuring = TRUE;
	config->sleeping = FALSE;
}

void save_data (fsm_t* this)
{
	sensor_buf_t data2save;

	fsm_sensor_t* punt = (fsm_sensor_t*) this;
	sensor_t* config = punt->param;

	data2save.Device_ID = config->Device_ID;
	data2save.Sensor_ID = config->Sensor_ID;
	data2save.alarm = config->alarm;
	data2save.error = config->error;
	data2save.measure = config->data_average;
	data2save.threshold_H = config->threshold_H;
	data2save.threshold_L = config->threshold_L;

	save_new_data(data2save);

	data_saved_flg = TRUE;
}

fsm_trans_t trans_sensor[] = {
  { Pre_setup, timer_adc,      Setup,     setting_up},
  { Setup,    timer_setup,     Measure,   measuring},
  { Measure,  timer_measure,   Measure,   measuring},
  { Measure,  average_counter, Process,   process_data},
  { Process,  error,           Warning,   alert},
  { Process,  no_error,        Saving,    save_data},
  { Warning,  warned, 		   Measure,   measuring},
  { Warning,  warning_limit,   Saving,    save_data},
  { Saving,   data_saved,      Sleeping,  sleep},
  { Sleeping,  timer_sleep,    Pre_setup, init_adc},
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

/**
  * @brief  Initialize the sensor to the params defined.
  * @param  Sensor initial parameters defined in main.c in macros.
  * @retval None
  */
void sensor_initialization(sensor_t* sensor, uint16_t Device_ID, uint8_t Sensor_ID, uint16_t supply_Pin, uint16_t threshold_L, uint16_t threshold_H, uint16_t threshold_Max, uint16_t setup_period, uint16_t sleep_period, uint16_t measure_period, uint16_t measure_average)
{
	/*SENSOR CONTROL*/
	sensor->active = TRUE;
	sensor->data_recovered = 0;
	sensor->data_average = 0;
	sensor->alarm = 0;
	sensor->error = 0;
	sensor->measuring = FALSE;
	sensor->sleeping = FALSE;
	sensor->setup_timer = 0;
	sensor->sleep_timer = 0;
	sensor->warning_count = 0;
	sensor->measure_timer = 0;
	sensor->measure_count = 0;

	/*SENSOR PARAMS*/
	sensor->Device_ID = Device_ID;
	sensor->Sensor_ID = Sensor_ID;
	sensor->supply_Pin = supply_Pin;
	sensor->threshold_L = threshold_L;
	sensor->threshold_H = threshold_H;
	sensor->threshold_Max = threshold_Max;
	sensor->setup_period = setup_period;
	sensor->sleep_period = sleep_period;
	sensor->measure_period = measure_period;
	sensor->measure_average = measure_average;
}
