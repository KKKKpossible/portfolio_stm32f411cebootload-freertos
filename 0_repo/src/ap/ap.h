/*
 * ap.h
 *
 *  Created on: Mar 6, 2023
 *      Author: ksj10
 */

#ifndef AP_AP_H_
#define AP_AP_H_


#include "hw.h"
#include "cmsis_os.h"
#include "ap_flash.h"
#include "ap_boot_loader.h"


void apInit(void);
void apMain(void);


#endif /* AP_AP_H_ */
