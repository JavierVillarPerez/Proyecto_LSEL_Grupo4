/*
 ============================================================================
 Name        : Tx_project.c
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Sensor Acquisition, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sensor_acq.h"



t_device req_sensor_data()
{
	t_device ret_data;

	ret_data.sensor1 = sensor_data1();
	ret_data.sensor2 = sensor_data2();
	ret_data.sensor3 = sensor_data3();

	return ret_data;
}


t_sensor sensor_data1()
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.ID = 1;
	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 10;
	sensor_value.alarm = 0;

	return sensor_value;
}

t_sensor sensor_data2()
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.ID = 2;
	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 20;
	sensor_value.alarm = 0;

	return sensor_value;
}

t_sensor sensor_data3()
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.ID = 3;
	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 30;
	sensor_value.alarm = 0;

	return sensor_value;
}
