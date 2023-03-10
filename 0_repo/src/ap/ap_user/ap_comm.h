/*
 * ap_comm.h
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_COMM_H_
#define AP_AP_USER_AP_COMM_H_


#include "ap.h"


typedef struct
{
    uint8_t buff[DEF_COMM_BUFF_LENGTH];
    int head;
    int tail;
    uint32_t whole_length;
}ap_comm_t;


extern ap_comm_t ap_comm_inst;


void apCommInit(void);
void apCommPushByte(uint8_t data);
uint8_t apCommPopByte(void);


#endif /* AP_AP_USER_AP_COMM_H_ */
