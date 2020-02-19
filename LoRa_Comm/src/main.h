/*
 * main.h
 *
 *  Created on: 16 feb. 2020
 *      Author: villa
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <time.h>

typedef enum
{
	FALSE = 0,
	TRUE
}t_bool;

typedef struct s_date
{
	int year;
	int month;
	int day;
	int hour;
	int minutes;
}t_date;

typedef struct s_sensor
{
	t_date timestamp;
	int measure;
	int ID;
	t_bool alarm;
}t_sensor;

typedef struct s_device
{
	t_sensor sensor1;
	t_sensor sensor2;
	t_sensor sensor3;
}t_device;


enum connexion_state
{
	WAIT
};

t_bool check_alarm();



#endif /* MAIN_H_ */
