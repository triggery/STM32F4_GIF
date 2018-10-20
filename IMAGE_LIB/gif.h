/*
 * gif.h
 *
 *  Created on: 20 апр. 2018 г.
 *      Author: dmitry
 */

#ifndef IMAGE_LIB_GIF_H_
#define IMAGE_LIB_GIF_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include "ff.h"

typedef struct gd_Palette {
    int size;
    uint8_t colors[0x100 * 3];
} gd_Palette;

typedef struct gd_GCE {
    uint16_t delay;
    uint8_t tindex;
    uint8_t disposal;
    int transparency;
} gd_GCE;

typedef struct gd_GIF {
	FIL *fp;
    off_t anim_start;
    uint16_t width, height;
    uint16_t depth;
    uint16_t loop_count;

    gd_GCE gce;
    gd_Palette *palette;
    gd_Palette lct, gct;

    uint16_t fx, fy, fw, fh;
    uint8_t bgindex;
    uint8_t bgcolor;
    uint8_t *frame;
} gd_GIF;

gd_GIF *gd_open_gif(const char *fname);
int gd_get_frame(gd_GIF *gif);
void gd_render_frame(gd_GIF *gif, uint8_t *buffer);
void gd_rewind(gd_GIF *gif);
void gd_close_gif(gd_GIF *gif);

#ifdef __cplusplus
}
#endif

#endif /* IMAGE_LIB_GIF_H_ */
