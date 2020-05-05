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
#include "stdio.h"
#include "SX1278.h"

#define PORT 8080

rbuf_t data_ring_buff;
SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

SPI_HandleTypeDef hspi1;


/*DEBUG FUNCTION*/
//int _write(int file, char *ptr, int len);


void LoRa_initialization(uint8_t init)
{
	uint8_t ret;

	/*Initialize ring buffer*/
	ringbuf_init(&data_ring_buff, RBUF_SIZE);

	//Initialize LoRa module
	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi1;
	SX1278.readBytes=0;
	SX1278.rxBuffer[0]=0;
	SX1278.hw = &SX1278_hw;

	//printf("Configuring LoRa module\r\n");
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8,
			SX1278_LORA_BW_20_8KHZ, 10);
	//printf("Done configuring LoRaModule\r\n");
	while (ret!=1){
		if (init == 1) {
			ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		} else {
			ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000); //tiene que valer 1
		}
		//printf("ret: %d\n", ret);
	}
}

void send_data(void)
{
	sensor_buf_t data;
	uint8_t ret;
	char buffer[64];
	uint8_t message_length;

	data = ringbuf_get(&data_ring_buff); //Take data from ring buffer.

	/*Send data by LoRa*/
	message_length = sprintf(buffer, "AquaSmart %d %d %d %d %d %d %d", data.Device_ID, data.Sensor_ID, data.measure, data.alarm, data.error, data.threshold_L, data.threshold_H);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
	//printf("Sending %s\r\n", buffer);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) buffer, message_length, 2000);
}

void receive_data(void)
{
	uint8_t ret;
	char buffer[64];

	ret = SX1278_LoRaRxPacket(&SX1278);
	if (ret > 0) {
		SX1278_read(&SX1278, (uint8_t *) buffer, ret);
		//printf("Content (%d): %s\r\n", ret, buffer);
	}
}

void save_new_data(sensor_buf_t data)
{
	ringbuf_put(&data_ring_buff, data);
}

/*DEBUG FUNCTION*/
//int _write(int file, char *ptr, int len) {
//	int i;
//	for (i = 0; i < len; i++) {
//		ITM_SendChar(*ptr++);
//	}
//	return len;
//}


