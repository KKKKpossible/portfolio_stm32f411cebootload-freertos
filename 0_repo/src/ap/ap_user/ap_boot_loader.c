/*
 * ap_boot_loader.c
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */


#include "ap_boot_loader.h"


#define WAIT_FOR_BOOT_MODE_TIME_TICK 10000


boot_t boot_inst;


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
    boot_inst.writing_address = boot_inst.start_address;
    apFlashWriteData(boot_inst.writing_address, boot_inst.file_writer, boot_inst.received_file_length);
    boot_inst.writing_address += boot_inst.received_file_length;
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
    apSysStateOperate();
}
