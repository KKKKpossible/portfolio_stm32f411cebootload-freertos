/*
 * bsp.h
 *
 *  Created on: Mar 6, 2023
 *      Author: ksj10
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_


#include "stm32f4xx.h"


void bspInit(void);
void SystemClock_Config(void);
void Error_Handler(void);

void delay(uint32_t tick);
uint32_t millis(void);

#endif /* BSP_BSP_H_ */
