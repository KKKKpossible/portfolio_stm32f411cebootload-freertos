/*
 * ap_comm.c
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */


#include "ap_comm.h"


ap_comm_t ap_comm_inst;



void apCommInit(void)
{

}

void apCommPushByte(uint8_t data)
{
    ap_comm_inst.buff[ap_comm_inst.head] = data;
    ap_comm_inst.head += 1;
    ap_comm_inst.head %= DEF_COMM_BUFF_LENGTH;
}

uint8_t apCommPopByte(void)
{
    uint8_t ret = ap_comm_inst.buff[ap_comm_inst.tail];
    ap_comm_inst.tail += 1;
    ap_comm_inst.tail %= DEF_COMM_BUFF_LENGTH;

    return ret;
}
