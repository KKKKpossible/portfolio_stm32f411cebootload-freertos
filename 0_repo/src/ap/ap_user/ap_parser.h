/*
 * ap_parser.h
 *
 *  Created on: Mar 9, 2023
 *      Author: ksj10
 */

#ifndef AP_AP_USER_AP_PARSER_H_
#define AP_AP_USER_AP_PARSER_H_


#include "ap.h"


typedef struct
{
    uint8_t buff[DEF_PARSER_BUFF_LENGTH];
    int head;
    int tail;
    uint32_t whole_length;
}parser_t;


extern parser_t parser_inst;


void apParserInit(void);
void apParserPush(uint8_t data);


#endif /* AP_AP_USER_AP_PARSER_H_ */
