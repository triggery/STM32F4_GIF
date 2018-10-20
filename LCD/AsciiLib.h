#ifndef __AsciiLib_H
#define __AsciiLib_H  

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stdint.h"

/* Private function prototypes -----------------------------------------------*/
void GetASCIICode1(unsigned char* pBuffer,unsigned char ASCII);
void GetASCIICode2(unsigned char* pBuffer,unsigned char ASCII);
void GetASCIICode3(short int * pBuffer,unsigned char ASCII);

void GetASCIICode4(unsigned char* pBuffer,unsigned char ASCII);
void GetASCIICode5(short int * pBuffer,unsigned char ASCII);

#ifdef __cplusplus
}
#endif

#endif 

