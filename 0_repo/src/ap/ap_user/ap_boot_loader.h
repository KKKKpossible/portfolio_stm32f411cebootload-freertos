/*
 * ap_boot_loader.h
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_BOOT_LOADER_H_
#define AP_AP_USER_AP_BOOT_LOADER_H_


#include "ap.h"


#define AP_BOOT_FILE_WRITER_LENGTH (255U)

typedef struct
{
    uint32_t start_address;
    uint8_t file_writer[AP_BOOT_FILE_WRITER_LENGTH];
    uint32_t start_tick;
    uint32_t writing_address;
    int rx_file_length;
    int received_file_length;
}boot_t;


extern boot_t boot_inst;


void apBootInit(void);
void apBootSaveFirmware(void);
void apBootJumpFirmware(void);
void apBootVertifyFirmware(void);


#endif /* AP_AP_USER_AP_BOOT_LOADER_H_ */
