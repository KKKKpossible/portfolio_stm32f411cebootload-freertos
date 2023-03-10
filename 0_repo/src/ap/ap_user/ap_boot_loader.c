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
    uint32_t now_tick = millis();

//    while(millis() - now_tick < WAIT_FOR_BOOT_MODE_TIME_TICK)
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
    if(ap_comm_inst.head != ap_comm_inst.tail)
    {
        apParserPush(apCommPopByte());
    }
}
