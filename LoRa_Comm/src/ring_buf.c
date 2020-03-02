/*
 ============================================================================
 Name        : ring_buf.c
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : Ring buffer for communication.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ring_buf.h"

#define NEXT(A) (A+1)==(RBUF_SIZE)?0:A+1 //Si la siguiente posición es el tamaño máximo del buffer se apunta al primero de nuevo.


/*RingBuffer initialization*/
void ringbuf_init(rbuf_t *_this, int size)
{
   rbuf_t *p = (rbuf_t*)_this->buf;

   if( (p == NULL) || (size!=sizeof(_this->buf))){

	 p=NULL;
     size = sizeof(_this->buf);
   }
  _this->head = 0;
  _this->tail = 0;
}

/*RingBuffer put value*/
void ringbuf_put(rbuf_t* _this, device_buf_t *item)
{
	_this->buf[_this->head] = *item;
	if(NEXT(_this->head) != 0)
	{
		_this->head++;
	}
	else _this->head = 0;
}

/*RingBuffer Get value*/
device_buf_t ringbuf_get(rbuf_t* _this, device_buf_t *item)
{

	if (_this->tail != _this->head)
    {

		*item = _this->buf[_this->tail];
		if (NEXT(_this->tail) != 0)
		{
			_this->tail++;
		}
		else _this->tail = 0;
    	}
	return *item;
}