#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#include "SSD1289.h"
#include "TouchLib.h"
#include "GesturesHandler.h"
#include "my_str_lib.h"
#include "math.h"

#include "stm32_ub_usb_msc_host.h"
#include "ff.h"

#include "FILE_MAN/file_man.h"
#include "audio_dac.h"
#include "dispatcher.h"

#include "USART/usart_init.h"
#include "icons_menu.h"
#include "main.h"
#include "gif.h"

#define CONVERT_TO_565(R, G, B) 	( ( (R << 8) & 0xF800) | ((G << 3) & 0x7E0) | ((B >> 3) & 0x1F) )

void LEDS_InitFunc(void);

uint16_t getColor565(uint8_t R, uint8_t G, uint8_t B)
{
	return (uint16_t)( ((R << 8) & 0xF800) | ((G << 3) & 0x7E0) | ((B >> 3) & 0x1F) );
}

int main(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
    	while (1);
    }

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);

	LCD_Init();
    LCD_SetTextColor(BLACK);
    LCD_SetBackColor(WHITE);
    LCD_Clear(LCD_BackColor());

	//LCD_StringLine(15, 15, "hello");

    LEDS_InitFunc();

	// USER BUTTON INIT
    GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	uint8_t *frame, *color;
	int ret, i, j;

	UB_USB_MSC_HOST_Init();

    while( UB_USB_MSC_HOST_Do()!=USB_MSC_DEV_CONNECTED );

	if( UB_Fatfs_Mount(USB_0) == FATFS_OK )
	{
		nextLinePos();
		FileMan_Init();
		FileMan_OpenFolder("/");
		//FileMan_Show();

		gd_GIF *gif = gd_open_gif("test.gif");
		if(gif == NULL)
			putErrorString("error: not open file!!!");
		frame = (uint8_t *)malloc(gif->width * gif->height * 3);
		if(!frame)
			putErrorString("error: not allocate frame bufer!!!");

		LCD_SetPORightUp_Win( (240/2)-gif->height/2, (320/2)+gif->width/2, gif->width, gif->height );
		LCD_WriteReg(LCD_REG_17, 0x6830); // AM=0; ID[1:0]=01
		LCD_WriteRAM_Prepare();

		while(1) {
			ret = gd_get_frame(gif);
			if(ret == -1)
				putErrorString("error: can't get frame!!!");
			gd_render_frame(gif, frame);

			color = frame;

			/*LCD_PutValue("0: ", (int)color[0], 3, nextLinePos());
			LCD_PutValue("1: ", (int)color[1], 3, nextLinePos());
			LCD_PutValue("2: ", (int)color[2], 3, nextLinePos());
			LCD_PutValue("3: ", (int)color[3], 3, nextLinePos());
			LCD_PutValue("4: ", (int)color[4], 3, nextLinePos());
			LCD_PutValue("5: ", (int)color[5], 3, nextLinePos());*/

			for (i = 0; i < gif->height; i++) {
				for (j = 0; j < gif->width; j++) {
					//								R		  G		    B
					//uint16_t pixel565 = getColor565(color[0], 0x50, color[2]);
					LCD_WriteRAM(getColor565(color[0], color[1], color[2]));	// 0xC063
					color += 3;
				}
			}
			//Delay(50);
	        if (ret == 0)
	            gd_rewind(gif);
		}

    	LCD_WriteReg(LCD_REG_17, 0x6830);
    	LCD_WriteReg(0x0044,0xEF00); // Horizontal position = 239
    	LCD_WriteReg(0x0045,0x0000); // start positions = 0

		free(frame);
		gd_close_gif(gif);
	}

	//init_USART1();
	//putString_USART("hello!!! \n");
	//printf("Start process handler\n");

	while(1)
    {

    }
}

void LEDS_InitFunc(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/** LEDS GPIO INIT **/
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
