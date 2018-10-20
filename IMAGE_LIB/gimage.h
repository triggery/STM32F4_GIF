/*
 * gimage.h
 *
 *  Created on: 4 мая 2017 г.
 *      Author: dmitry
 */

#ifndef MY_LIB_GIMAGE_H_
#define MY_LIB_GIMAGE_H_

#include <integer.h>
#include <stdint.h>
#include <stdbool.h>
#include "ff.h"
#include "gdisp.h"

typedef uint16_t	gdispImageType;

#define GDISP_IMAGE_TYPE_UNKNOWN	0
#define GDISP_IMAGE_TYPE_NATIVE		1
#define GDISP_IMAGE_TYPE_GIF		2
#define GDISP_IMAGE_TYPE_BMP		3
#define GDISP_IMAGE_TYPE_JPG		4
#define GDISP_IMAGE_TYPE_PNG		5

typedef struct gdispImage {
	gdispImageType		type;
	color_t				bgcolor;
	coord_t				width, height;
	FIL					*fp;
	DWORD 				frame0pos;
	WORD				bitsperpixel;
}gdispImage;


typedef uint16_t	gdispImageError;
	#define GDISP_IMAGE_ERR_OK					0
	#define GDISP_IMAGE_ERR_UNRECOVERABLE		0x8000
	#define GDISP_IMAGE_ERR_BADFORMAT			(GDISP_IMAGE_ERR_UNRECOVERABLE+1)
	#define GDISP_IMAGE_ERR_BADDATA				(GDISP_IMAGE_ERR_UNRECOVERABLE+2)
	#define GDISP_IMAGE_ERR_UNSUPPORTED			(GDISP_IMAGE_ERR_UNRECOVERABLE+3)
	#define GDISP_IMAGE_ERR_UNSUPPORTED_OK		3
	#define GDISP_IMAGE_ERR_NOMEMORY			(GDISP_IMAGE_ERR_UNRECOVERABLE+4)
	#define GDISP_IMAGE_ERR_NOSUCHFILE			(GDISP_IMAGE_ERR_UNRECOVERABLE+5)


#ifdef __cplusplus
 extern "C" {
#endif



#ifdef __cplusplus
}
#endif



#endif /* MY_LIB_GIMAGE_H_ */
