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
  if(NEXT(_this->head) != 0){ //SOLO GUARDA LOS RBUF_SIZE PRIMEROS. LOS DEMÁS LOS DESCARTA POR AHORA
  _this->buf[_this->head] = *item;
  _this->head++;
  }
}

//void ringbuf_put(rbuf_t* _this, device_buf_t *item)
//{
//  if(NEXT(_this->buf[_this->head]) != 0){
//  _this->buf[_this->head] = item;
//  _this->head++;
//  }
//}

/*RingBuffer Get value*/
device_buf_t ringbuf_get(rbuf_t* _this, device_buf_t *item)
{
//  if (NEXT(_this->tail) != 0)
    if (_this->tail != _this->head)
    {
      // get item element
      *item = _this->buf[_this->tail];
      _this->tail++;
    }
	return *item;
}
