/*
 * GesturesHandler.h
 *
 *  Created on: 5 февр. 2017 г.
 *      Author: dmitry
 */

#ifndef LCD_GESTURESHANDLER_H_
#define LCD_GESTURESHANDLER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "TouchLib.h"
#include "SSD1289.h"

typedef enum {
	TOUCH_MODE_COUNT_STOP = 0,
	TOUCH_MODE_COUNT_START = 1,
	TOUCH_MODE_TOUCH_DOT,
	TOUCH_MODE_MOVE
}TOUCH_MODE_STATUS_ENUM;

typedef struct {
	TOUCH_MODE_STATUS_ENUM mode;
	Point point;
}TOUCH_EVENT;

void setTimingDel(int count);
bool retStatusTimTouch(void);
void pulseCountFunc(void);

//TOUCH_MODE_STATUS_ENUM getTouchMode(Point *p);
TOUCH_EVENT getTouchMode( void );

#ifdef __cplusplus
}
#endif
#endif /* LCD_GESTURESHANDLER_H_ */
