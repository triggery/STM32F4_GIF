/*
 * gdisp_image_bmp.c
 *
 *  Created on: 4 мая 2017 г.
 *      Author: dmitry
 */
#include "gdisp_image_bmp.h"
#include "SSD1289.h"
#include "ff.h"
#include "libBMP_tiny.h"
#include "my_str_lib.h"

// #TODO необх. хадать число, а в завис-ти от разрад-ти 16/24 картинки
// задавать длину буфера так, чтоб делилась на 2 или 3 байта

#define IMG_BUF_LEN					552
#define CONVERT_TO_565(R, G, B) 	( ( (R << 8) & 0xF800) | ((G << 3) & 0x7E0) | ((B >> 3) & 0x1F) )

typedef struct gdispImagePrivate_BMP {
	uint8_t		bmpflags;
	coord_t 	height;
	coord_t		width;
	uint8_t		bitsperpixel;
	size_t		frame0pos;
}gdispImagePrivate_BMP;


gdispImageError gdispImageOpen_BMP(gdispImage *img)
{
	UINT cnt;
	BITMAPFILEHEADER file_h;
	BITMAPINFOHEADER info_h;

	f_read ( img->fp, &file_h, sizeof(BITMAPFILEHEADER), &cnt );

	/*f_read ( img->fp, &file_h.bfType, sizeof(WORD), &cnt );
	f_read ( img->fp, &file_h.bfSize, sizeof(DWORD), &cnt );
	f_read ( img->fp, &file_h.bfReserved1, sizeof(WORD), &cnt );
	f_read ( img->fp, &file_h.bfReserved2, sizeof(WORD), &cnt );
	f_read ( img->fp, &file_h.bfOffBits, sizeof(DWORD), &cnt );*/

	if(file_h.bfType != 0x4D42) {
		f_close(img->fp);						// Clean up the private data area
		return GDISP_IMAGE_ERR_UNSUPPORTED;		// Not supported
		//return GDISP_IMAGE_ERR_BADFORMAT;
	}

	f_read ( img->fp, &info_h, sizeof(BITMAPINFOHEADER), &cnt );

	img->height = info_h.biHeight;
	img->width = info_h.biWidth;
	img->bitsperpixel = info_h.biBitCount;
	img->frame0pos = file_h.bfOffBits;

	img->type = GDISP_IMAGE_TYPE_BMP;
	return GDISP_IMAGE_ERR_OK;
}


gdispImageError gdispGImageDraw_BMP(gdispImage *img, coord_t x, coord_t y)
{
	uint8_t img_buff[IMG_BUF_LEN];
	UINT cnt;
	FRESULT res;
	uint32_t mindex;

	if( img->type != GDISP_IMAGE_TYPE_BMP )
		return GDISP_IMAGE_ERR_BADFORMAT;
	res = f_lseek( img->fp, img->frame0pos );
	if( res )
		return GDISP_IMAGE_ERR_NOSUCHFILE;

	if( img->height == 240 )
	{
		LCD_SetLORightDown_Win(239, 319, 240, 320);
		LCD_WriteReg(LCD_REG_17, 0x6048); // AM=0; ID[1:0]=11
	}
	else
	{
		LCD_Clear(LCD_BackColor());
		LCD_SetPORightUp_Win( (240/2)-img->height/2, (320/2)+img->width/2, img->width, img->height );
		LCD_WriteReg(LCD_REG_17, 0x6810); // AM=0; ID[1:0]=01
	}

	LCD_WriteRAM_Prepare();

	if ( img->bitsperpixel == 16 )
	{
		do {
			res = f_read ( img->fp, &img_buff[0], IMG_BUF_LEN, &cnt );
			for(mindex = 0; mindex < cnt ; mindex+=2) {
				LCD_WriteRAM( (uint16_t)( img_buff[mindex] | (img_buff[mindex+1]) << 8 ) );
			}
		}while(res == 0 && cnt == IMG_BUF_LEN);
	}
	else // if( img->bitsperpixel == 24 )
	{
		do {
			res = f_read ( img->fp, &img_buff[0], IMG_BUF_LEN, &cnt );
			for(mindex = 0; mindex < cnt ; mindex+=3) {
				//									(	RRRRR			,	GGGGGG			,	BBBBB		)
				LCD_WriteRAM( (uint16_t)CONVERT_TO_565(img_buff[mindex+2], img_buff[mindex+1], img_buff[mindex]) );
			}
		}while(res == 0 && cnt == IMG_BUF_LEN);
	}

	LCD_WriteReg(LCD_REG_17, 0x6830);
	LCD_WriteReg(0x0044,0xEF00); // Horizontal position = 239
	LCD_WriteReg(0x0045,0x0000); // start positions = 0

	f_close(img->fp);
	return GDISP_IMAGE_ERR_OK;
}

void gdispShowErrorImage(gdispImageError gerr)
{
	char buff[5];
	/*if ( gerr != GDISP_IMAGE_ERR_OK )
	{*/		
		itoa_m( gerr, &buff[0] );
		LCD_PutString( 10,     20, "ERROR: " );
		LCD_PutString( 10+7*8, 20, &buff[0]  );
	//}
}