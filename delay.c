/*
 * delay.c
 *
 *  Created on: 30 янв. 2015 г.
 *      Author: dmitry
 */
#include "delay.h"

static __IO uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

