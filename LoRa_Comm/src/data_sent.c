/*
 ============================================================================
 Name        : Tx_project.c
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Data sent, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data_sent.h"


void send_sensor_data(t_device data)
{
	printf("Enviando al Gateway la información corresponediente al dispositivo: %d\n" , data.ID);
	printf("Medidas: \n");
	printf("Sensor 1: %d\n", data.sensor.sensor1);
	printf("Sensor 2: %d (Where 0 means not active and 1 means active)\n", data.sensor.sensor2);
	printf("Sensor 3: %d\n\n\n", data.sensor.sensor3);

	printf("Esperando por la siguiente muestra...\n");

}
