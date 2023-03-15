/*
 * ap_sys_state.h
 *
 *  Created on: 2023. 3. 10.
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_SYS_STATE_H_
#define AP_AP_USER_AP_SYS_STATE_H_


#include "ap.h"


typedef struct
{
    uint8_t cmd_state;
    uint32_t firmware_start_address;
}ap_sys_t;


extern ap_sys_t ap_sys_inst;


void apSysStateInit(void);
void apSysStateOperate(void);
void txLoop(void);

#endif /* AP_AP_USER_AP_SYS_STATE_H_ */
