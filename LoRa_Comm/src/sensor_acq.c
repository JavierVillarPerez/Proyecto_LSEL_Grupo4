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

	ret_data.sensor.sensor1 = sensor_data1();
	ret_data.sensor.sensor2 = sensor_data2();
	ret_data.sensor.sensor3 = sensor_data3();
	ret_data.ID = 1234;

	return ret_data;
}


int sensor_data1()
{
	int sensor_value = 0;

	sensor_value = 10;
	return sensor_value;
}

t_bool sensor_data2()
{
	t_bool sensor_value = FALSE;

	sensor_value = TRUE;
	return sensor_value;
}

int sensor_data3()
{
	int sensor_value = 0;

	sensor_value = 30;
	return sensor_value;
}
