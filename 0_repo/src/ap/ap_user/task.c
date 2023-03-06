/*
 * task.c
 *
 *  Created on: Mar 6, 2023
 *      Author: ksj10
 */


#include "task.h"


static osThreadId defaultTaskHandle;


static void StartDefaultTask(void const * argument);


void initTask(void)
{
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
}

static void StartDefaultTask(void const * argument)
{
	while(true)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		osDelay(100);
	}
}
