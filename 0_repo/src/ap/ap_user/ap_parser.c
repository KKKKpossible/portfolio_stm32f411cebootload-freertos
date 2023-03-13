/*
 * ap_parser.c
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */


#include "ap_parser.h"


enum
{
    TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT
}cmd_enum;


parser_t parser_inst;



void apParserInit(void)
{

}

void apParserFetchComm(void)
{
    bool catched_special_char = false;

    while(ap_comm_inst.head != ap_comm_inst.tail)
    {
        uint8_t data = apCommPopByte();
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
                break;
            default:
                apParserPush(data);
                break;
        }
    }
}

void apParserPush(uint8_t data)
{
    parser_inst.buff[parser_inst.head] = data;
    parser_inst.head += 1;
    parser_inst.head %= DEF_COMM_BUFF_LENGTH;
}

void apParseOperate(void)
{
    if(parser_inst.head != parser_inst.tail)
    {
        switch(parser_inst.buff[AP_PAR_PIE_CMD])
        {
            case TX_START:
            {
                ap_sys_inst.cmd_state |= 1 << TX_START;
                break;
            }
            case FILE_WRITE:
            {
                ap_sys_inst.cmd_state |= 1 << FILE_WRITE;
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
}
