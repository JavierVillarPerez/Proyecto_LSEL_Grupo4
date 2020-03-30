/*
 * LoRa_comm.h
 *
 *  Created on: 29 mar. 2020
 *      Author: villa
 */

#ifndef INC_LORA_COMM_H_
#define INC_LORA_COMM_H_

#include "main.h"
#include "ring_buf.h"
#include "fsm.h"



fsm_t* LoRa_initialization(uint16_t period);
void save_new_data(sensor_buf_t data);


#endif /* INC_LORA_COMM_H_ */
