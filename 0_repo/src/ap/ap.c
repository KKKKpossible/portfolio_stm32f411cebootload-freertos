/*
 * ap.c
 *
 *  Created on: Mar 6, 2023
 *      Author: ksj10
 */


#include "ap.h"


void apInit(void)
{
	hwInit();
	apFlashInit();
	apBootInit();

	initTask();
	osKernelStart();
}

void apMain(void)
{
	while(true)
	{

	}
}
