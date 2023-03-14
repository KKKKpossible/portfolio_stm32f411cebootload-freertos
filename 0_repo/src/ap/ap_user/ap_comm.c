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

void apCommRxPushByte(uint8_t data)
{
    ap_comm_inst.rx_buff[ap_comm_inst.rx_head] = data;
    ap_comm_inst.rx_head += 1;
    ap_comm_inst.rx_head %= DEF_COMM_BUFF_LENGTH;
}

uint8_t apCommRxPopByte(void)
{
    uint8_t ret = ap_comm_inst.rx_buff[ap_comm_inst.rx_tail];
    ap_comm_inst.rx_tail += 1;
    ap_comm_inst.rx_tail %= DEF_COMM_BUFF_LENGTH;

    return ret;
}

void apCommTxPushByte(uint8_t data, bool check_special)
{
    ap_comm_inst.tx_buff[ap_comm_inst.tx_head] = data;
    ap_comm_inst.tx_head += 1;
    ap_comm_inst.tx_head %= DEF_COMM_BUFF_LENGTH;

    if(check_special == true)
    {
        if(data == AP_PAR_SBE_SPECIAL)
        {
            ap_comm_inst.tx_buff[ap_comm_inst.tx_head] = AP_PAR_SBE_CLEAR;
            ap_comm_inst.tx_head += 1;
            ap_comm_inst.tx_head %= DEF_COMM_BUFF_LENGTH;
        }
    }
}

uint8_t apCommTxPopByte(void)
{
    uint8_t ret = ap_comm_inst.tx_buff[ap_comm_inst.tx_tail];
    ap_comm_inst.tx_tail += 1;
    ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;

    return ret;
}

uint16_t apCommGetTxDataLength(void)
{
    int length = ap_comm_inst.tx_head - ap_comm_inst.tx_tail;
    length += DEF_COMM_BUFF_LENGTH;
    length %= DEF_COMM_BUFF_LENGTH;

    return length;
}
