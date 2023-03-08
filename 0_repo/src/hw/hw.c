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

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	MX_USB_DEVICE_Init();
}
