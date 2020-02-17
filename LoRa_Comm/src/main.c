/*
 ============================================================================
 Name        : Tx_project.c
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Hello LoRa, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "sensor_acq.h"
#include "data_sent.h"

int main(void) {
	t_device a_data;

	printf("Iniciando proceso...\n");
	while(1)
	{
		a_data = req_sensor_data();

		DELAY_ms(2000); //2 sec delay (measure timing simulation)

		send_sensor_data(a_data);

		DELAY_ms(10000); //Task delay
	}
}


void DELAY_ms(uint16_t ms_count)
{
	while(ms_count!=0)
	 {
		DELAY_us(1000);	 //DELAY_us is called to generate 1ms delay
		 ms_count--;
		}

}

void DELAY_us(uint16_t us_count)
 {
    while(us_count!=0)
      {
		 us_count--;
	   }
  }
