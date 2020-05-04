/*
 * ring_buf.h
 *
 *  Created on: Apr 11, 2020
 *      Author: villa
 */

#ifndef INC_RING_BUF_H_
#define INC_RING_BUF_H_

#include "main.h"

#define RBUF_SIZE 3

typedef struct s_date
{
	int year;
	int month;
	int day;
	int hour;
	int minutes;
}t_date;


typedef struct device_buf_s{
	uint16_t Device_ID;
	uint8_t  Sensor_ID;
	uint16_t measure;
	uint8_t alarm;		// warning signal
	uint8_t error;		// sensor´s error
	uint16_t threshold_L;
	uint16_t threshold_H;
}sensor_buf_t;


typedef struct ring_buf_s{
  sensor_buf_t buf[RBUF_SIZE];
  int head;         // new data is written at this position in the buffer
  int tail;         // data is read from this position in the buffer
} rbuf_t;



void ringbuf_init(rbuf_t *_this, int size);

// fetch a byte from the queue at tail
sensor_buf_t ringbuf_get(rbuf_t *_this);

// insert a byte to the queue at head
void ringbuf_put(rbuf_t* _this, sensor_buf_t item);



#endif /* INC_RING_BUF_H_ */
