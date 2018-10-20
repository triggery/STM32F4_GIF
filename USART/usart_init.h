/*
 * usart_init.h
 *
 *  Created on: 20 апр. 2018 г.
 *      Author: dmitry
 */

#ifndef USART_USART_INIT_H_
#define USART_USART_INIT_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_usart.h"

void init_USART1(void);
void putChar_USARTx(USART_TypeDef* USARTx, char data );
void putString_USART(const char *string);

#ifdef __cplusplus
}
#endif

#endif /* USART_USART_INIT_H_ */
