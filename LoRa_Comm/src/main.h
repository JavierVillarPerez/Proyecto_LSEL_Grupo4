/*
 * main.h
 *
 *  Created on: 16 feb. 2020
 *      Author: villa
 */

#ifndef MAIN_H_
#define MAIN_H_


typedef enum
{
	FALSE = 0,
	TRUE
}t_bool;


typedef struct s_sensor
{
	int sensor1;
	t_bool sensor2;
	int sensor3;
}t_sensor;


typedef struct s_device
{
	t_sensor sensor;
	int ID;
}t_device;


void DELAY_ms(uint16_t ms_count);
void DELAY_us(uint16_t us_count);


#endif /* MAIN_H_ */
