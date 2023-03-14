using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1
{
    internal class Def
    {
        int trans_data_length_max = 255 - 12;
        int trans_data_one_time = 8;

        public int Trans_data_length_max { get => trans_data_length_max; set => trans_data_length_max = value; }
        public int Trans_data_one_time { get => trans_data_one_time; set => trans_data_one_time = value; }

        // last index - 2 = checksum
        // last index - 1 = end special index
        // last index = endindex
        public enum COMM_PROTOCOL_INDEX
        {
            START_SPECIAL_INDEX = 0,
            START_INDEX,
            CMD_INDEX,
            DATA_L_MSB_INDEX,
            DATA_L_LSB_INDEX,
            DATA_START_ONLYCMDCHECKSUM_INDEX,
            ONLYCMD_END_SPECIAL_INDEX,
            ONLYCMD_END_INDEX,
        };

        public enum CMD_ENUM
        {
            TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT
        };

        public enum PACKET
        {
            SPECIAL_CHAR = '*',
            START_CHAR = 0,
            END_CHAR = 1,
            SPECIAL_CLEAR = 2,
        }
    }
}
