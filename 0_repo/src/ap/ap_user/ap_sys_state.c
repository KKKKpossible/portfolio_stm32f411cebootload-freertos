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
            txLoop();
        }
        // 1. FILE_WRITE
        if((ap_sys_inst.cmd_state & (1 << FILE_WRITE)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << FILE_WRITE));

            apBootSaveFirmware();
            retCmdAct(FILE_WRITE);
            txLoop();
        }
        // 2. FILE_READ
        if((ap_sys_inst.cmd_state & (1 << FILE_READ)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << FILE_READ));
            retCmdAct(FILE_READ);
            txLoop();
        }
        // 3. TX_END
        if((ap_sys_inst.cmd_state & (1 << TX_END)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << TX_END));

            uint8_t checksum = 0;
            uint8_t cmd_buff = TX_END;
            uint8_t length = 4;
            uint32_t time_cost = millis() - boot_inst.start_tick;

            apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
            apCommTxPushByte(AP_PAR_SBE_START, false);

            apCommTxPushByte(cmd_buff, true);
            checksum = cmd_buff;
            apCommTxPushByte(length, true);
            checksum ^= length;

            apCommTxPushByte((uint8_t)(time_cost >> 24), true);
            checksum ^= (uint8_t)(time_cost >> 24);
            apCommTxPushByte((uint8_t)(time_cost >> 16), true);
            checksum ^= (uint8_t)(time_cost >> 16);
            apCommTxPushByte((uint8_t)(time_cost >> 8), true);
            checksum ^= (uint8_t)(time_cost >> 8);
            apCommTxPushByte((uint8_t)(time_cost), true);
            checksum ^= (uint8_t)(time_cost);

            apCommTxPushByte(checksum, true);

            apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
            apCommTxPushByte(AP_PAR_SBE_STOP, false);
            txLoop();
        }
        // 4. ERR_CHECKSUM
        if((ap_sys_inst.cmd_state & (1 << ERR_CHECKSUM)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << ERR_CHECKSUM));
            retCmdAct(ERR_CHECKSUM);
            txLoop();
        }
        // 5. ERR_TIMEOUT
        if((ap_sys_inst.cmd_state & (1 << ERR_TIMEOUT)) != 0)
        {
            ap_sys_inst.cmd_state &= (uint8_t)(~(1 << ERR_TIMEOUT));
            retCmdAct(ERR_TIMEOUT);
            txLoop();
        }
    }
}

void txLoop(void)
{
    int tx_length_left;

    tx_length_left = apCommGetTxDataLength();
    if(tx_length_left > 0)
    {
        if(tx_length_left > DEF_COMM_TX_WRITER_LENGTH)
        {
            for(int i = 0; i < DEF_COMM_TX_WRITER_LENGTH; i++)
            {
                ap_comm_inst.tx_writer[i] = apCommTxPopByte();
            }
            CDC_Transmit_FS(ap_comm_inst.tx_writer, DEF_COMM_TX_WRITER_LENGTH);
        }
        else
        {
            for(int i = 0; i < tx_length_left; i++)
            {
                ap_comm_inst.tx_writer[i] = apCommTxPopByte();
            }
            CDC_Transmit_FS(ap_comm_inst.tx_writer, DEF_COMM_TX_WRITER_LENGTH);
        }
    }

}

static void retCmdAct(uint8_t cmd)
{
    uint8_t checksum = 0;
    uint8_t cmd_buff = cmd;
    uint8_t length = 0;

    apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
    apCommTxPushByte(AP_PAR_SBE_START, false);

    apCommTxPushByte(cmd_buff, true);
    checksum = cmd_buff;
    apCommTxPushByte(length, true);  // data length lsb
    checksum ^= length;
    apCommTxPushByte(checksum, true);

    apCommTxPushByte(AP_PAR_SBE_SPECIAL, false);
    apCommTxPushByte(AP_PAR_SBE_STOP, false);

}
