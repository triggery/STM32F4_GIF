/*
 * libBMP_tiny.h
 *
 *  Created on: 20 марта 2015 г.
 *      Author: dmitry
 */

#ifndef LIBBMP_TINY_H_
#define LIBBMP_TINY_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "integer.h"
#include "stdint.h"
#include <stdbool.h>
#include "ff.h"

#define SIZE_ICON_BUFF 1024	// 32x32 pixels

#pragma pack(push, 1)
typedef struct _BITMAPFILEHEADER {	// смещение
	WORD bfType;			// 0
	DWORD bfSize;			// 2
	WORD bfReserved1;		// 6
	WORD bfReserved2;		// 8
	DWORD bfOffBits;		// 10
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER
{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
/*  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;*/
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#pragma pack(pop)

typedef struct _SIZE_IMAGE {
  uint16_t Width;
  uint16_t Height;
} SIZE_IMAGE;

enum {
  BMP_PORTRAIT = 0,
  BMP_LANDSCAPE = 1
}TYPE_ORIENTATION;

bool ReadIconToBuff(const char *fname, uint16_t *BitmapBuff, SIZE_IMAGE *size );
bool LCD_PutBMP_FS(const char *fname);

bool ShowImageSize(const char *fname, uint8_t Xpos, uint16_t Ypos);

#ifdef __cplusplus
}
#endif
#endif /* LIBBMP_TINY_H_ */
