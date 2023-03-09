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


void apParserInit(void)
{

}

void apParserPush(uint8_t data)
{
    parser_inst.buff[parser_inst.head] = data;
    parser_inst.head += 1;
    parser_inst.head %= DEF_PARSER_BUFF_LENGTH;
}

void apParseOperate(void)
{
    if(parser_inst.head != parser_inst.tail)
    {
        switch(parser_inst.tail)
        {
            case TX_START:
            {
                break;
            }
            case FILE_WRITE:
            {
                break;
            }
            case FILE_READ:
            {
                break;
            }
            case TX_END:
            {
                break;
            }
            default:
                break;
        }
    }
}
