/*
 * delay.h
 *
 *  Created on: 30 янв. 2015 г.
 *      Author: dmitry
 */

#ifndef DELAY_H_
#define DELAY_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H_ */
