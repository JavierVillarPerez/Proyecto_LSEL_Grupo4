/*
 ============================================================================
 Name        : sensor_acq.c
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
#include "ring_buf.h"


device_buf_t req_sensor_data(int count)
{
	device_buf_t ret_data;

	ret_data.ID = count;
	ret_data.sensor1 = sensor_data1(count);
	ret_data.sensor2 = sensor_data2(count);
	ret_data.sensor3 = sensor_data3(count);

	return ret_data;
}


t_sensor sensor_data1(int increment)
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 10 + 10*increment;
	sensor_value.alarm = 0;

	return sensor_value;
}

t_sensor sensor_data2(int increment)
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 20 + 10*increment;
	sensor_value.alarm = 0;

	return sensor_value;
}

t_sensor sensor_data3(int increment)
{
	t_sensor sensor_value;
	time_t now ;
	struct tm *local = localtime(&now);

	sensor_value.timestamp.year = local->tm_year;
	sensor_value.timestamp.month = local->tm_mon;
	sensor_value.timestamp.day = local->tm_mday;
	sensor_value.timestamp.hour = local->tm_hour;
	sensor_value.timestamp.minutes = local->tm_min;
	sensor_value.measure = 30 + 10*increment;
	sensor_value.alarm = 0;

	return sensor_value;
}
