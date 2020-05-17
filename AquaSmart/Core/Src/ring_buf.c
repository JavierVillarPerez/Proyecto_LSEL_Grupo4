/*
 * ring_buf.c
 *
 * Description: Ring buffer for data management.
 * 				This configuration allows save/read data from a ring buffer with 2 pointers to
 * 				tail and head of the buffer.
 *
 *  Created on: 9 mar. 2020
 *      Author: Javier Villar
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ring_buf.h"

#define NEXT(A) (A+1)==(RBUF_SIZE)?0:A+1 /*If next position is the buffer max size, the pointer point to the initial position 0*/


/**
  * @brief  Initialize the ring buffer:
  * 		- Head: Pointer to the head of the buffer. Head is increased after a writing.
  * 		- Tail: Pointer to the tail of the buffer. Tail is increased after a reading.
  * @param: rbuf_t type variable and ring buffer size.
  * @retval None
  */
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

/**
  * @brief  Write a value into the ring buffer:
  * 		- Head: Pointer to the head of the buffer. Head is increased after a writing.
  * 		- Tail: Pointer to the tail of the buffer. Tail is increased after a reading.
  * @param: rbuf_t*: Pointer to the ring buffer.
  * 		sensor_buf_t: Variable to write in the ring buffer.
  * @retval None
  */void ringbuf_put(rbuf_t* _this, sensor_buf_t item)
{
  if((NEXT(_this->head) != 0) && (NEXT(_this->head)!= _this->tail)){
  _this->buf[_this->head] = item;
  _this->head++;
  }
  else
  {
	  _this->buf[_this->head] = item;
	  if(NEXT(_this->head) == _this->tail)
	  {
		  _this->head = _this->tail;
		  _this->tail = NEXT(_this->tail);
	  }
	  else
	  {
		  _this->head = 0;
		  _this->tail = RBUF_SIZE - 2; /*Full buffer, reload.*/
	  }
  }
}

  /**
    * @brief  Read a value from the ring buffer:
    * 		- Head: Pointer to the head of the buffer. Head is increased after a writing.
    * 		- Tail: Pointer to the tail of the buffer. Tail is increased after a reading.
    * @param: rbuf_t*: Pointer to the ring buffer.
    * @retval None*/
sensor_buf_t ringbuf_get(rbuf_t* _this)
{
	sensor_buf_t item;

	if (_this->tail != _this->head)
	{
		item = _this->buf[_this->tail];
		if (NEXT(_this->tail) != 0)
		{
			_this->tail++;
		}
		else
		{
			_this->tail = 0;
		}
    }
	return item;
}
