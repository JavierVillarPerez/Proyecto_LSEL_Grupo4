/*
 ============================================================================
 Name        : Tx_project.c
 Author      : Javier Villar P�rez
 Version     :
 Copyright   : Your copyright notice
 Description : Hello LoRa, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "fsm.h"
#include "sensor_acq.h"
#include "data_sent.h"
#include <assert.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include "ring_buf.h"


static int timer_finished (fsm_t* this);
static int alarm_ON(fsm_t* this);
static void send_data(fsm_t* this);



device_buf_t a_data;
t_bool alarm_state;
static int timer;
rbuf_t LoRa_ring_buff;
int count; //VAR FOR DEBUG.

static void timer_isr(union sigval arg)
{
  timer = 1;
}

static void timer_start(int ms)
{
  timer_t timerid;
  struct itimerspec spec;
  struct sigevent se;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = &timerid;
  se.sigev_notify_function = timer_isr;
  se.sigev_notify_attributes = NULL;
  spec.it_value.tv_sec = ms / 1000;
  spec.it_value.tv_nsec = (ms % 1000) * 1000000;
  spec.it_interval.tv_sec = 0;
  spec.it_interval.tv_nsec = 0;
  timer_create (CLOCK_REALTIME, &se, &timerid);
  timer_settime (timerid, 0, &spec, NULL);
}


/* FSM definition
 * Finite State Machine to define the system behavior.
 * Description: The system implements a sudden movements detector. A blue LED will indicate when hardware is connected blinking
 * once per second. The user button enable/disable the sudden movements detector, enabling/disabling the accelerometer reading.
 * States:
 * 			- wait_500: Initial state. The user button activation is disabled 500ms once it has been pressed.
 * 			- wait_button: System waits for the enabling/disabling sudden movements detector.
 * 			- check_edge: The system checks if the button has been correctly pressed.
 * */
static fsm_trans_t send_wireless[]= {
  { WAIT, timer_finished, WAIT, send_data},
  { WAIT, alarm_ON, WAIT, send_data},
  {-1, NULL, -1, NULL },
};



static int timer_finished(fsm_t* this)
{
	return timer;
}

static int alarm_ON(fsm_t* this)
{
	if(check_alarm() == TRUE) return 1;
	else return 0;
}

static void send_data(fsm_t* this)
{
	printf("Reading data...\n");
	a_data = req_sensor_data(count); //ONLY FOR TEST, DATA WILL BE MODIFIED BY THE SENSOR FSM
	printf("contador: %d\n",count);
	if (count < 9)
	{
		count++;
	}
	printf("Sending data...\n");
	ringbuf_put(&LoRa_ring_buff, &a_data);
	
	printf("Devices saved in the ring buffer: \n");
	for(int i = 0; i<9; i++)
	{
		printf("Device ID: %d\n", LoRa_ring_buff.buf[LoRa_ring_buff.tail].ID);
		LoRa_ring_buff.tail++;
	}
	LoRa_ring_buff.tail = 0;
	timer = 0;
	timer_start(2000);

}


/* Timer decrement: res = a - b*/
void timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}

/* Timer increase: res = a + b */
void timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec + b->tv_sec
    + a->tv_usec / 1000000 + b->tv_usec / 1000000;
  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;
}




// Wait until next_activation (absolute time)
void delay_until (struct timeval* next_activation)
{
  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);
}


/* Main loop */
int main ()
{
  struct timeval clk_period = { 0, 250 * 1000 };
  struct timeval next_activation;

	alarm_state = FALSE;
	timer_start(10);

	fsm_t* send_wireless_fsm = fsm_new (send_wireless);
	ringbuf_init(&LoRa_ring_buff, RBUF_SIZE);
	printf("Iniciando proceso...\n");

	while(1)
	{
		fsm_fire(send_wireless_fsm);
		gettimeofday (&next_activation, NULL);
		timeval_add (&next_activation, &next_activation, &clk_period);
		delay_until (&next_activation);
	}
}

t_bool check_alarm()
{
	return alarm_state; //This variable will be modified by Sensor FSM.
}
