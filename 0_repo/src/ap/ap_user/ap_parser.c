/*
 * ap_parser.c
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */


#include "ap_parser.h"


enum
{
    SBE_START = 0,
    SBE_STOP = 1,
    SBE_SPECIAL = '*',
}special_bit_enum;

enum
{
    TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT
}cmd_enum;


parser_t parser_inst;


static void apParseOperate(void);


void apParserInit(void)
{

}

void apParserPush(uint8_t data)
{
    parser_inst.buff[parser_inst.head] = data;
    parser_inst.head += 1;
    parser_inst.head %= DEF_COMM_BUFF_LENGTH;

    apParseOperate();
}

static void apParseOperate(void)
{
    if(parser_inst.head != parser_inst.tail)
    {
        switch(parser_inst.tail)
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
