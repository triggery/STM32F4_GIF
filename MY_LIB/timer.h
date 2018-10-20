/*
 * timer.h
 *
 *  Created on: 5 мая 2017 г.
 *      Author: dmitry
 */

#ifndef MY_LIB_TIMER_H_
#define MY_LIB_TIMER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

typedef struct TIME_STRUCT {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
}TIME_STRUCT;

void timer1sec(void);
bool getEventTim1sec(void);
int getTimSeconds(void);

TIME_STRUCT getTime(void);

#ifdef __cplusplus
}
#endif

#endif /* MY_LIB_TIMER_H_ */
