/*
 * gdisp.h
 *
 *  Created on: 4 мая 2017 г.
 *      Author: dmitry
 */

#ifndef MY_LIB_GDISP_H_
#define MY_LIB_GDISP_H_

#include "integer.h"
#include "stdint.h"

typedef uint16_t color_t;
typedef uint16_t coord_t;
typedef color_t		pixel_t;

typedef struct point {
	coord_t x;
	coord_t y;
} point, point_t;


#ifdef __cplusplus
 extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif /* MY_LIB_GDISP_H_ */
