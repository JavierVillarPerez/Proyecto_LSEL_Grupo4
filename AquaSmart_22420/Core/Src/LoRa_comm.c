/*
 * fsm_LoRa.c
 *
 *  Created on: Mar 28, 2020
 *      Author: villa
 */

#include "LoRa_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "ring_buf.h"


#define PORT 8080

rbuf_t data_ring_buff;
long LoRa_timer;
uint16_t LoRa_period;

//enum
//{
//	WAIT,
//}comm_state;



//int timer_finished(fsm_t* this)
//{
//	return(HAL_GetTick() >= +LoRa_timer);
//}

sensor_buf_t send_data(void)
{
	 sensor_buf_t data;

	 data = ringbuf_get(&data_ring_buff);
	 return data;
}
//
//fsm_trans_t send_wireless[] = {
//	  { WAIT, timer_finished, WAIT, send_data},
//	  {-1, NULL, -1, NULL },
//};

//fsm_t* LoRa_initialization(uint16_t period)
void LoRa_initialization(void)
{
//	fsm_t* fsm_LoRa;
	ringbuf_init(&data_ring_buff, RBUF_SIZE);
//	fsm_LoRa = fsm_new(send_wireless);
//	LoRa_timer = HAL_GetTick() + period;
//	LoRa_period = period;

//	return fsm_LoRa;
}

void save_new_data(sensor_buf_t data)
{
	ringbuf_put(&data_ring_buff, data);
}


//	/*SEND DATA TO THE SERVER*/
//    send(sock , &device , sizeof(device) , 0 );
//    printf("Device sent ID: %d\n", device.ID);
//
//	timer = 0;
//	/*TIME OF 2 seconds for the next sent*/
//	timer_start(2000);
//
//}
//

