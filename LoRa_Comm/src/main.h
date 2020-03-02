/*
 ============================================================================
 Name        : main.h
 Author      : Javier Villar Pérez
 Version     :
 Copyright   : Your copyright notice
 Description : main template
 ============================================================================
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <time.h>

typedef enum
{
	FALSE = 0,
	TRUE
}t_bool;


enum connexion_state
{
	WAIT
};

t_bool check_alarm();



#endif /* MAIN_H_ */
