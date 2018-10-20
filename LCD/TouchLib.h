/*
 * TouchLib.h
 * Touch ads7846 stm32f4
 *  Created on: 13.04.2016
 *      Author: dmitry
 */

#ifndef TOUCHLIB_H_
#define TOUCHLIB_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "SSD1289.h"

#define TOUCH_NOT_RELEASED			!(GPIOD->IDR & GPIO_Pin_6)
#define TOUCH_IS_RELEASED			(GPIOD->IDR & GPIO_Pin_6)

#define    Xmax	3900
#define    Xmin 200

#define    Ymax	3900
#define    Ymin	310

typedef enum {
	NOT_TOUCH = 0,
	TOUCH_PRESSED = 1,
	TOUCH_RELEASED
}STATUS_TOCH;

void TouchInit ( void );
Point GetTouchPoint(STATUS_TOCH *status);
int GetCoordValue(uint16_t *x, uint16_t *y);

STATUS_TOCH getTouchStatus(void);
void setTouchStatus(STATUS_TOCH status);

STATUS_TOCH getTouchPointAdvanced(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif /* TOUCHLIB_H_ */
