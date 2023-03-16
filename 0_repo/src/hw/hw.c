/*
 * hw.c
 *
 *  Created on: Mar 6, 2023
 *      Author: ksj10
 */


#include "hw.h"


void hwInit(void)
{
	bspInit();


    int test = 0;
	if(test == 1)
	{
	    HAL_RCC_DeInit();
	    for(int i = 0; i < 8; i++)
	    {
	        NVIC->ICER[i] = 0xFFFFFFFF;
	        __DSB();
	        __ISB();
	    }
	    SysTick->CTRL = 0;
	    void (**jumper)(void) = (void (**)(void))(0x8020000 + 4);
	    (*jumper)();
	}
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	MX_USB_DEVICE_Init();
}

