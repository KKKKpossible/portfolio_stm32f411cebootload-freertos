/*
 * ap_boot_loader.h
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_BOOT_LOADER_H_
#define AP_AP_USER_AP_BOOT_LOADER_H_


#include "ap.h"


#define WAIT_FOR_BOOT_MODE_TIME_TICK 10000
#define AP_BOOT_FILE_WRITER_LENGTH (255U)


typedef struct
{
    uint32_t rx_start_address;
    uint8_t file_writer[AP_BOOT_FILE_WRITER_LENGTH];
    uint32_t start_tick;
    uint32_t writing_address;
    int rx_file_length;
    int received_file_length;

    uint32_t tag_version_data;
    uint32_t tag_firmware_address_data;

    uint32_t tag_address_start;
    uint32_t tag_address_end;
    uint32_t tag_firmware_address_start;
    uint32_t tag_firmware_address_end;
    uint32_t tag_version_address_start;
    uint32_t tag_version_address_end;


    uint32_t firmware_address_start;
    uint32_t firmware_address_end;

    void (**jumper)(void);
}boot_t;


extern boot_t boot_inst;


void apBootInit(void);
void apBootSaveFirmware(void);
void apBootJumpFirmware(void);
void apBootVertifyFirmware(void);
void apBootDeleteFirmware(void);
void apBootSaveTag(void);
void apBootLoadTag(void);
void apBootDeleteTag(void);
void apBootBspDeInit(void);

#endif /* AP_AP_USER_AP_BOOT_LOADER_H_ */
