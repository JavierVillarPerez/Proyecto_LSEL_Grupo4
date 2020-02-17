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
		printf("Enviando al Gateway la información corresponediente al dispositivo: %d\n" , data.sensor1.ID);
		printf("Medida: %d\n", data.sensor1.measure);
		printf("Fecha: %d:%d %d/%d/%d\n", data.sensor1.timestamp.hour, data.sensor1.timestamp.minutes, data.sensor1.timestamp.day, data.sensor1.timestamp.month, data.sensor1.timestamp.year);
		printf("Alarma: %d\n", data.sensor1.alarm);

		printf("Enviando al Gateway la información corresponediente al dispositivo: %d\n" , data.sensor2.ID);
		printf("Medida: %d\n", data.sensor2.measure);
		printf("Fecha: %d:%d %d/%d/%d\n", data.sensor2.timestamp.hour, data.sensor2.timestamp.minutes, data.sensor2.timestamp.day, data.sensor2.timestamp.month, data.sensor2.timestamp.year);
		printf("Alarma: %d\n", data.sensor2.alarm);

		printf("Enviando al Gateway la información corresponediente al dispositivo: %d\n" , data.sensor3.ID);
		printf("Medida: %d\n", data.sensor3.measure);
		printf("Fecha: %d:%d %d/%d/%d\n", data.sensor3.timestamp.hour, data.sensor3.timestamp.minutes, data.sensor3.timestamp.day, data.sensor3.timestamp.month, data.sensor3.timestamp.year);
		printf("Alarma: %d\n", data.sensor3.alarm);

		printf("Esperando por la siguiente muestra...\n");

}
