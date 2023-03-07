/*
 * ap_flash.h
 *
 *  Created on: 2023. 3. 7.
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_FLASH_H_
#define AP_AP_USER_AP_FLASH_H_


#include "ap.h"


typedef struct
{
	uint32_t start_address;
	uint32_t size;
}flash_t;


void apFlashInit(void);
void apFlashWriteData(uint32_t start_address, uint8_t *p_data, uint32_t length);
void apFlashErase(uint32_t start_address, uint32_t length);
uint8_t apFlashReadByte(uint32_t address);


#endif /* AP_AP_USER_AP_FLASH_H_ */
