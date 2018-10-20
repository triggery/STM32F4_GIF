/*
 * timer.c
 *
 *  Created on: 5 мая 2017 г.
 *      Author: dmitry
 */

#include "timer.h"

int n = 1000;
bool TIC = 0;

int sec = 0;
TIME_STRUCT time;
void HendleTim(void);
//bool timerStart = 0;

void timer1sec(void)
{
	if( n > 0 )
	{
		n--;
	}
	else
	{
		n = 1000;
		TIC = 1;
		//sec++;
		HendleTim();
	}
}

bool getEventTim1sec(void)
{
	if ( TIC == 1 ) {
		TIC = 0;
		return 1;
	}
	else
		return 0;
}

int getTimSeconds(void)
{
	return sec;
}

void HendleTim(void)
{
	if(sec < 60)
		sec++;
	else
	{
		sec = 0;
		if(time.min < 60)
			time.min++;
		else
		{
			time.min=0;
			if( time.hour < 24 )
				time.hour++;
			else
			{
				time.hour = 0;
			}
		}
	}
}

TIME_STRUCT getTime(void)
{
	return time;
}
