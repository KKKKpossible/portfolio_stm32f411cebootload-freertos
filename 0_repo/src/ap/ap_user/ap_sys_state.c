/*
 * ap_sys_state.c
 *
 *  Created on: 2023. 3. 10.
 *      Author: ksj10
 */


#include "ap_sys_state.h"
#include "usbd_cdc_if.h"


ap_sys_t ap_sys_inst;


static void retCmdAct(uint8_t cmd);


void apSysStateInit(void)
{

}

void apSysStateOperate(void)
{
    while(ap_sys_inst.cmd_state != 0)
    {
        // 0. TX_START
        // 1. FILE_WRITE
        // 2. FILE_READ
        // 3. TX_END
        // 4. ERR_CHECKSUM
        // 5. ERR_TIMEOUT

        // 0. TX_START
        if((ap_sys_inst.cmd_state & (1 << TX_START)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << TX_START));
            retCmdAct(TX_START);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());
            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
        // 1. FILE_WRITE
        if((ap_sys_inst.cmd_state & (1 << FILE_WRITE)) != 0)
        {
            ap_sys_inst.cmd_state |= ~(1 << FILE_WRITE);
            retCmdAct(FILE_WRITE);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());

            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
        // 2. FILE_READ
        if((ap_sys_inst.cmd_state & (1 << FILE_READ)) != 0)
        {
            ap_sys_inst.cmd_state |= ~(1 << FILE_READ);
            retCmdAct(FILE_READ);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());

            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
        // 3. TX_END
        if((ap_sys_inst.cmd_state & (1 << TX_END)) != 0)
        {
            ap_sys_inst.cmd_state |= ~(1 << TX_END);
            retCmdAct(TX_END);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());

            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
        // 4. ERR_CHECKSUM
        if((ap_sys_inst.cmd_state & (1 << ERR_CHECKSUM)) != 0)
        {
            ap_sys_inst.cmd_state |= ~(1 << ERR_CHECKSUM);
            retCmdAct(ERR_CHECKSUM);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());

            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
        // 5. ERR_TIMEOUT
        if((ap_sys_inst.cmd_state & (1 << ERR_TIMEOUT)) != 0)
        {
            ap_sys_inst.cmd_state |= ~(1 << ERR_TIMEOUT);
            retCmdAct(ERR_TIMEOUT);
            CDC_Transmit_FS(&ap_comm_inst.tx_buff[ap_comm_inst.tx_tail], apCommGetTxDataLength());

            ap_comm_inst.tx_tail += apCommGetTxDataLength();
            ap_comm_inst.tx_tail %= DEF_COMM_BUFF_LENGTH;
        }
    }
}

static void retCmdAct(uint8_t cmd)
{
    uint8_t checksum = 0;
    uint8_t cmd_buff = cmd;
    uint16_t length = 0;


    apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
    apCommTxPushByte(AP_PAR_SBE_START, false);

    apCommTxPushByte(cmd_buff, true);
    checksum = cmd_buff;
    apCommTxPushByte((length >> 8), true);  // data length msb
    checksum ^= (length >> 8);
    apCommTxPushByte((uint8_t)(length & 0xFF), true);  // data length lsb
    checksum ^= (uint8_t)(length & 0xFF);
    apCommTxPushByte(checksum, true);

    apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
    apCommTxPushByte(AP_PAR_SBE_STOP, false);

}
