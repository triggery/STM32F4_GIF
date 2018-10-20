/*
 * my_str_lib.h
 *
 *  Created on: 29 окт. 2013
 *      Author: dmitry
 */

#ifndef MY_STR_LIB_H_
#define MY_STR_LIB_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

 typedef struct _FLOAT_BUFF_TYPE {
 	char int_num[7];
 	char float_num[3];
 	char sign;
 }FLOAT_BUFF_TYPE;

char* strcopy_m (char *destination, const char *source);
uint8_t itoa_m(int n, char *s);

#ifdef __cplusplus
}
#endif

#endif /* MY_STR_LIB_H_ */
