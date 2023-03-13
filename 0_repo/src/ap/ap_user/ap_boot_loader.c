/*
 * ap_boot_loader.c
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */


#include "ap_boot_loader.h"


#define WAIT_FOR_BOOT_MODE_TIME_TICK 10000


static void apBootInitLoop(void);


void apBootInit(void)
{
    while(true)
    {
        apBootInitLoop();
    }
}

void apBootSaveFirmware(void)
{

}

void apBootJumpFirmware(void)
{

}

void apBootVertifyFirmware(void)
{

}

static void apBootInitLoop(void)
{
    apParserFetchComm();
}
