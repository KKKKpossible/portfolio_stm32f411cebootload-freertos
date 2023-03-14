/*
 * ap_parser.h
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_PARSER_H_
#define AP_AP_USER_AP_PARSER_H_


#include "ap.h"


enum cmd_enum
{
    TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT
};

enum special_bit_enum
{
    AP_PAR_SBE_START = 0,
    AP_PAR_SBE_STOP = 1,
    AP_PAR_SBE_CLEAR = 2,
    AP_PAR_SBE_SPECIAL = '*',
};

enum protocol_index_enum
{
    AP_PAR_PIE_CMD = 0,
    AP_PAR_PIE_DATA_LENGTH_MSB,
    AP_PAR_PIE_DATA_LENGTH_LSB,
    AP_PAR_PIE_DATA_DATA_START,
};

typedef struct
{
    uint8_t buff[DEF_COMM_BUFF_LENGTH];
    int head;
    int tail;
}parser_t;


extern parser_t parser_inst;


void apParserInit(void);
void apParseOperate(void);
void apParserFetchComm(void);
void apParserPush(uint8_t data);


#endif /* AP_AP_USER_AP_PARSER_H_ */
