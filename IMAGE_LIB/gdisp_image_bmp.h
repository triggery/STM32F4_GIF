/*
 * gdisp_image_bmp.h
 *
 *  Created on: 4 мая 2017 г.
 *      Author: dmitry
 */

#ifndef IMAGE_LIB_GDISP_IMAGE_BMP_H_
#define IMAGE_LIB_GDISP_IMAGE_BMP_H_

#include "gdisp.h"
#include "gimage.h"


#ifdef __cplusplus
 extern "C" {
#endif

 gdispImageError gdispImageOpen_BMP(gdispImage *img);
 gdispImageError gdispGImageDraw_BMP(gdispImage *img, coord_t x, coord_t y);

 void gdispShowErrorImage(gdispImageError gerr);

#ifdef __cplusplus
}
#endif

#endif /* IMAGE_LIB_GDISP_IMAGE_BMP_H_ */
