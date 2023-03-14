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
    uint8_t rx_buff[DEF_COMM_BUFF_LENGTH];
    int rx_head;
    int rx_tail;
    uint32_t rx_whole_length;
    uint8_t tx_buff[DEF_COMM_BUFF_LENGTH];
    int tx_head;
    int tx_tail;
    uint32_t tx_whole_length;
}ap_comm_t;


extern ap_comm_t ap_comm_inst;


void apCommInit(void);
void apCommRxPushByte(uint8_t data);
uint8_t apCommRxPopByte(void);
void apCommTxPushByte(uint8_t data, bool check_special);
uint8_t apCommTxPopByte(void);
uint16_t apCommGetTxDataLength(void);


#endif /* AP_AP_USER_AP_COMM_H_ */
