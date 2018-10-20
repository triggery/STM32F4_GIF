/*
 * GesturesHandler.c
 *
 *  Created on: 5 февр. 2017 г.
 *      Author: dmitry
 */

/****	HOW IT USED		****
 *
 * 		Point radpoint;
 * 		TOUCH_EVENT touchEvent = getTouchMode();
    	Point point = touchEvent.point;

    	if ( touchEvent.mode == TOUCH_MODE_TOUCH_DOT )
    	{
			// point.X, point.Y
    	}
    	else if ( touchEvent.mode == TOUCH_MODE_MOVE )
    	{
			radpoint = GetTouchPoint(&status);
			// radpoint.Y, radpoint.X
    	}
*/

#include "GesturesHandler.h"
#include "stm32f4xx_gpio.h"

#define TIMING_WAIT_DOT_TOUCH	100

int timingDelay = 0;
void setTimingDel(int count)
{
	timingDelay = count;
}

bool retStatusTimTouch(void)
{
	return timingDelay? 1 : 0;
}

void pulseCountFunc(void)
{
	if( timingDelay > 0)
		timingDelay--;
}

TOUCH_EVENT touchModestatus = { TOUCH_MODE_COUNT_STOP, {0,0} };
TOUCH_EVENT getTouchMode( void )
{
	STATUS_TOCH status;

	if( getTouchStatus() == TOUCH_PRESSED )
	{
		switch( touchModestatus.mode )
		{

		case TOUCH_MODE_COUNT_STOP:
			touchModestatus.point = GetTouchPoint(&status);
			if( status == TOUCH_PRESSED ) {
				setTimingDel(TIMING_WAIT_DOT_TOUCH);
				touchModestatus.mode = TOUCH_MODE_COUNT_START;
			}
			break;

		case TOUCH_MODE_COUNT_START:
			if( retStatusTimTouch() == 0 )
			{
				if( TOUCH_IS_RELEASED && ( getTouchStatus() == TOUCH_PRESSED ) ) // произошло касание!!!
				{   // отпускаем сенсор, режим касания
					touchModestatus.mode = TOUCH_MODE_TOUCH_DOT;
				}
				else // значит переходим в режим перемещения
				{
					touchModestatus.mode = TOUCH_MODE_MOVE;
				}
			}
			break;

		case TOUCH_MODE_TOUCH_DOT:

			setTouchStatus(NOT_TOUCH);
			touchModestatus.mode = TOUCH_MODE_COUNT_STOP;
			break;

		case TOUCH_MODE_MOVE:
			if( !TOUCH_IS_RELEASED && ( getTouchStatus() == TOUCH_PRESSED ) ) // пока не произошел отрыв делаем что-то
			{

			}
			else {
				setTouchStatus(NOT_TOUCH);
				touchModestatus.mode = TOUCH_MODE_COUNT_STOP;
			}
			break;
		}
	}
	return touchModestatus;
}

/*
TOUCH_MODE_STATUS_ENUM statusPulseCount = TOUCH_MODE_COUNT_STOP;
TOUCH_MODE_STATUS_ENUM getTouchMode(Point *p)
{
	STATUS_TOCH status;

	if( getTouchStatus() == TOUCH_PRESSED )
	{
		switch( statusPulseCount )
		{

		case TOUCH_MODE_COUNT_STOP:
			*p = GetTouchPoint(&status);
			if( status == TOUCH_PRESSED ) {
				setTimingDel(100);
				statusPulseCount = TOUCH_MODE_COUNT_START;
			}
			break;

		case TOUCH_MODE_COUNT_START:
			if( retStatusTimTouch() == 0 )
			{
				if( TOUCH_IS_RELEASED && ( getTouchStatus() == TOUCH_PRESSED ) ) // произошло касание!!!
				{   // отпускаем сенсор, режим касания
					statusPulseCount = TOUCH_MODE_TOUCH_DOT;
				}
				else // значит переходим в режим перемещения
				{
					statusPulseCount = TOUCH_MODE_MOVE;
				}
			}
			break;

		case TOUCH_MODE_TOUCH_DOT:

			setTouchStatus(NOT_TOUCH);
			statusPulseCount = TOUCH_MODE_COUNT_STOP;
			break;

		case TOUCH_MODE_MOVE:
			if( !TOUCH_IS_RELEASED && ( getTouchStatus() == TOUCH_PRESSED ) ) // пока не произошел отрыв делаем что-то
			{

			}
			else {
				setTouchStatus(NOT_TOUCH);
				statusPulseCount = TOUCH_MODE_COUNT_STOP;
			}
			break;
		}
	}
	return statusPulseCount;
}*/
