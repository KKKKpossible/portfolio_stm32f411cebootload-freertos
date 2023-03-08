/*
 * ap_boot_loader.h
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_BOOT_LOADER_H_
#define AP_AP_USER_AP_BOOT_LOADER_H_


#include "ap.h"


void apBootInit(void);
void apBootSaveFirmware(void);
void apBootJumpFirmware(void);
void apBootVertifyFirmware(void);


#endif /* AP_AP_USER_AP_BOOT_LOADER_H_ */
