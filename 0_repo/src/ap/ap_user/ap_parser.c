/*
 * ap_parser.c
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */


#include "ap_parser.h"




parser_t parser_inst;


static bool checksumProcedure(void);


void apParserInit(void)
{

}

void apParserFetchComm(void)
{
    bool catched_special_char = false;

    while(ap_comm_inst.rx_head != ap_comm_inst.rx_tail)
    {
        uint8_t data = apCommRxPopByte();
        switch(data)
        {
            case AP_PAR_SBE_SPECIAL:
                catched_special_char = true;
                break;
            case AP_PAR_SBE_START:
                if(catched_special_char == true)
                {
                    parser_inst.head = 0;
                    parser_inst.tail = 0;
                }
                else
                {
                    apParserPush(data);
                }
                catched_special_char = false;
                break;
            case AP_PAR_SBE_STOP:
                if(catched_special_char == true)
                {
                    apParseOperate();
                }
                else
                {
                    apParserPush(data);
                }
                catched_special_char = false;
                break;
            case AP_PAR_SBE_CLEAR:
                if(catched_special_char == true)
                {
                    apParserPush(AP_PAR_SBE_SPECIAL);
                }
                else
                {
                    apParserPush(data);
                }
                catched_special_char = false;
                break;
            default:
                apParserPush(data);
                catched_special_char = false;
                break;
        }
    }
}

void apParserPush(uint8_t data)
{
    if(parser_inst.head < DEF_COMM_BUFF_LENGTH)
    {
        parser_inst.buff[parser_inst.head] = data;
        parser_inst.head += 1;
    }
}

// TODO: fetch data from parse buffer to instance
void apParseOperate(void)
{
    if(checksumProcedure() == false)
    {
        return;
    }

    int data_length = parser_inst.buff[AP_PAR_PIE_DATA_LENGTH];

    switch(parser_inst.buff[AP_PAR_PIE_CMD])
    {
        case TX_START:
        {
            ap_sys_inst.cmd_state |= 1 << TX_START;
            if(data_length == 4)
            {
                boot_inst.start_address |= (uint32_t)((parser_inst.buff[AP_PAR_PIE_DATA_START] << 24) & 0xFF000000);
                boot_inst.start_address |= (uint32_t)((parser_inst.buff[AP_PAR_PIE_DATA_START + 1]<< 16) & 0x00FF0000);
                boot_inst.start_address |= (uint32_t)((parser_inst.buff[AP_PAR_PIE_DATA_START + 2] << 8) & 0x0000FF00);
                boot_inst.start_address |= (uint32_t)((parser_inst.buff[AP_PAR_PIE_DATA_START + 3]) & 0x000000FF);

                memset(boot_inst.file_writer, '\0', sizeof(boot_inst.file_writer) / sizeof(uint8_t));
                boot_inst.rx_file_length = 0;
                boot_inst.start_tick = millis();
            }
            break;
        }
        case FILE_WRITE:
        {
            ap_sys_inst.cmd_state |= 1 << FILE_WRITE;
            boot_inst.received_file_length = 0;
            for(int i = 0; i < data_length; i++)
            {
                boot_inst.file_writer[i] = parser_inst.buff[AP_PAR_PIE_DATA_START + i];
                boot_inst.rx_file_length += 1;
                boot_inst.received_file_length += 1;
            }
            break;
        }
        case FILE_READ:
        {
            ap_sys_inst.cmd_state |= 1 << FILE_READ;
            break;
        }
        case TX_END:
        {
            ap_sys_inst.cmd_state |= 1 << TX_END;
            break;
        }
        default:
            break;
    }
}

static bool checksumProcedure(void)
{
    uint8_t checksum_rx = parser_inst.buff[parser_inst.head - 1];
    uint8_t checksum_buff = 0;

    for(int i = parser_inst.tail; i < parser_inst.head - 1; i++)
    {
        if(i == parser_inst.tail)
        {
            checksum_buff = parser_inst.buff[i];
        }
        else
        {
            checksum_buff ^= parser_inst.buff[i];
        }
    }

    if(checksum_rx != checksum_buff)
    {
        return false;
    }
    else
    {
        return true;
    }
}
