/*
 * main.h
 *
 *  Created on: 16 feb. 2020
 *      Author: villa
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
