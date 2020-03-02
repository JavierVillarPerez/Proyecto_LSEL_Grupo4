/*
 ============================================================================
 Name        : ring_buf.h
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Ring buffer library
 ============================================================================
 */

#ifndef RING_BUF_H_
#define RING_BUF_H_

#include "main.h"

#define RBUF_SIZE 10

typedef struct s_date
{
	int year;
	int month;
	int day;
	int hour;
	int minutes;
}t_date;


typedef struct s_sensor
{
	t_date timestamp;
	int measure;
	t_bool alarm;
}t_sensor;

typedef struct device_buf_s{
	int ID;
	t_sensor sensor1;
	t_sensor sensor2;
	t_sensor sensor3;
}device_buf_t;

typedef struct ring_buf_s{
  device_buf_t buf[RBUF_SIZE];
  int head;         // new data is written at this position in the buffer
  int tail;         // data is read from this position in the buffer
} rbuf_t;



void ringbuf_init(rbuf_t *_this, int size);

// fetch a byte from the queue at tail
device_buf_t ringbuf_get(rbuf_t *_this, device_buf_t *item);

// insert a byte to the queue at head
void ringbuf_put(rbuf_t* _this, device_buf_t *item);



#endif /* RING_BUF_H_ */
