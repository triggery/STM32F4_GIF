/*
 * TouchLib.c
 * Touch ads7846 stm32f4
 *  Created on: 13.04.2016
 *      Author: dmitry
 */
#include "TouchLib.h"
#include "stdint.h"
#include "stdbool.h"

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "SSD1289.h"
#include "my_str_lib.h"

#define TP_SELECT()			GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define TP_DESELECT()		GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define BAND_VALUES_X	( Xmax - Xmin )
#define DIVIDER_X		( ( BAND_VALUES_X/(DISP_H+1) ) + 1 )
#define OFFSET_X		10

#define BAND_VALUES_Y	( Ymax - Ymin )
#define DIVIDER_Y		( ( BAND_VALUES_Y/(DISP_W+1) ) + 1 )
#define OFFSET_Y		18

#define NUM_OF_DECODE	6

uint16_t TouchReadX(void);
uint16_t TouchReadY(void);

void TP_GetXY(uint16_t *x, uint16_t *y);
void TP_DelayUS(uint16_t ms);

STATUS_TOCH statusT = NOT_TOUCH;

void TouchInit ( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef  SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);	//SCLK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);	//MISO
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);	//MOSI

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_Cmd ( SPI2, DISABLE );
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init ( SPI2, &SPI_InitStructure );
	SPI_CalculateCRC ( SPI2, DISABLE );
	SPI_Cmd ( SPI2, ENABLE );

	//SPI_SSOutputCmd ( SPI2, DISABLE );
	//SPI_NSSInternalSoftwareConfig ( SPI2, SPI_NSSInternalSoft_Set );

	//CS - GPIOB_12
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	// Deactivate touch controller
	TP_DESELECT();

	//T_PEN - GPIOD_6
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
//**************External interrupts**************
//	GPIOD_6 - Line6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
    	EXTI_ClearITPendingBit(EXTI_Line6);
    	setTouchStatus(TOUCH_PRESSED); //TOUCH_RELEASED
	}
}

static void WR_CMD (uint8_t cmd)
{
  /* Wait for SPI2 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI2 data */
  SPI_I2S_SendData(SPI2,cmd);
  /* Wait for SPI2 data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI2 received data */
  SPI_I2S_ReceiveData(SPI2);
}

static uint16_t RD_AD(void)
{
  unsigned short buf,temp;
  /* Wait for SPI2 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI2 data */
  SPI_I2S_SendData(SPI2,0x0000);
  /* Wait for SPI2 data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI2 received data */
  temp = SPI_I2S_ReceiveData(SPI2);
  buf = temp<<8;

  TP_DelayUS(2);

  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI2 data */
  SPI_I2S_SendData(SPI2,0x0000);
  /* Wait for SPI2 data reception */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI2 received data */
  temp = SPI_I2S_ReceiveData(SPI2);
  buf |= temp;
  buf >>= 3;
  buf &= 0x0FFF;
  return buf;
}

uint16_t TouchReadX(void)
{
	uint16_t X;
	TP_SELECT();
	TP_DelayUS(2);
	WR_CMD(0x90);
	TP_DelayUS(2);
	X = RD_AD();
	TP_DESELECT();
	return X;
}

uint16_t TouchReadY(void)
{
	uint16_t Y;
	TP_SELECT();
	TP_DelayUS(2);
	WR_CMD(0xd0);
	TP_DelayUS(2);
	Y = RD_AD();
	TP_DESELECT();
	return Y;
}

void TP_GetXY(uint16_t *x, uint16_t *y)
{
	*x = TouchReadX();
	*y = TouchReadY();
}

int GetCoordValue(uint16_t *x, uint16_t *y)
{
	uint16_t count = 0;
	uint16_t valBuff [2][NUM_OF_DECODE] = { {0}, {0} };
	uint16_t pX, pY;

	while( count < NUM_OF_DECODE )
	{
		TP_GetXY( &pX, &pY );
		if( pX > Xmin && pX < Xmax && pY > Ymin && pY < Ymax )
		{
			valBuff[0][count] = pX;
			valBuff[1][count] = pY;
			count++;
		}
	}

	pX = 0;
	pY = 0;
	for(count = 0; count < NUM_OF_DECODE; count++ )
	{
		pX += valBuff[0][count];
		pY += valBuff[1][count];
	}
	*x = pX/NUM_OF_DECODE;
	*y = pY/NUM_OF_DECODE;
	return 0;
}

Point GetTouchPoint(STATUS_TOCH *status)
{
	Point point;
	uint16_t pX, pY;

	if ( getTouchPointAdvanced( &pX, &pY ) == TOUCH_PRESSED ) {
		point.X = (DISP_H + 1) - ( pX/DIVIDER_X - OFFSET_X );
		point.Y = pY/DIVIDER_Y - OFFSET_Y;
		*status = TOUCH_PRESSED;
	}
	else {
		point.X = 0;
		point.Y = 0;
		*status = NOT_TOUCH;
	}
	return point;
}

void TP_DelayUS(uint16_t ms)
{
	uint16_t c;

	for( c = 0; c < ms; c++)
	{
		uint16_t i = 12;
		while ( i-- );
	}
}

STATUS_TOCH getTouchStatus(void)
{
	return statusT;
}

void setTouchStatus(STATUS_TOCH status)
{
	statusT = status;
}
//-------------------------------------------------------------------------------

STATUS_TOCH getTouchPointAdvanced(uint16_t *x, uint16_t *y)
{
	uint16_t valBuff [2][NUM_OF_DECODE] = { {0}, {0} };
    uint16_t pX, pY;
    int count = 0;

	while( count < NUM_OF_DECODE )
	{
		if ( TOUCH_NOT_RELEASED ) {	//!(GPIOD->IDR & GPIO_Pin_6)
			TP_GetXY( &pX, &pY );
			if( pX > Xmin && pX < Xmax && pY > Ymin && pY < Ymax )
			{
				valBuff[0][count] = pX;
				valBuff[1][count] = pY;
				count++;
			}
		}
		else {
			/*LCD_PutStringShorten(10, nextLinePos(), "not touch");
			LCD_PutValue(" cnt=", count, 100,  currLinePos());*/
			return NOT_TOUCH;
		}
	}

	pX = 0;
	pY = 0;
	for(count = 0; count < NUM_OF_DECODE; count++ )
	{
		pX += valBuff[0][count];
		pY += valBuff[1][count];
	}
	*x = pX/NUM_OF_DECODE;
	*y = pY/NUM_OF_DECODE;
	//LCD_PutValue("cnt=", count, 10,  nextLinePos());
    return TOUCH_PRESSED;
}

