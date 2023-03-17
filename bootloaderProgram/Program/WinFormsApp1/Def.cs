using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1
{
    internal class Def
    {
        int trans_data_length_max = 255 - 10;
        int trans_data_one_time = 8;
        int button_state = 0;
        bool file_loaded = false;

        public int Trans_data_length_max { get => trans_data_length_max; set => trans_data_length_max = value; }
        public int Trans_data_one_time { get => trans_data_one_time; set => trans_data_one_time = value; }
        public int Button_state { get => button_state; set => button_state = value; }
        public bool File_loaded { get => file_loaded; set => file_loaded = value; }

        // last index - 2 = checksum
        // last index - 1 = end special index
        // last index = endindex
        public enum COMM_TX_PROTOCOL_INDEX
        {
            START_SPECIAL_INDEX = 0,
            START_INDEX,
            CMD_INDEX,
            DATA_LENGTH_INDEX,
            DATA_START_ONLYCMDCHECKSUM_INDEX,
            ONLYCMD_END_SPECIAL_INDEX,
            ONLYCMD_END_INDEX,
        };

        // last index = checksum
        public enum COMM_PARSE_PROTOCOL_INDEX
        {
            CMD_INDEX = 0,
            DATA_LENGTH,
            DATA_START,
        };

        public enum CMD_ENUM
        {
            TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT, ERR_FLASH_RANGE, CMD_LENGTH
        };

        public enum PACKET
        {
            SPECIAL_CHAR = '*',
            START_CHAR = 0,
            END_CHAR = 1,
            SPECIAL_CLEAR = 2,
        }

        public enum HEX_STRING_INDEX
        { 
            START = 0,
            LENGTH_START = 1,
            ADDRESS_START = 3,
            TYPE_START = 7,
            DATA_START = 9
        }

        public enum HEX_TYPE_ENUM
        { 
            DATA = 0,
            END = 1,
            ADDRESS = 4,
            MDK_ADDRESS = 5
        }

    }
}
