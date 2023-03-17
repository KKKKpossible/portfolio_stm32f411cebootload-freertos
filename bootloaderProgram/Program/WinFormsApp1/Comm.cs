using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1
{
    internal class Comm
    {
        UInt32 rx_time_out_tick = 0;

        bool parsing_start = false;
        SerialPort port = new SerialPort();
        
        
        List<byte> file_data_bin = new List<byte>();
        Queue<byte> file_writer = new Queue<byte>();
        List<string> file_data_hex = new List<string>();
        List<byte> file_data_parsed = new List<byte>();
        UInt32 start_address = 0;

        Queue<byte> transfer = new Queue<byte>();
        Queue<byte> receiver = new Queue<byte>();
        List<byte> parse_buffer = new List<byte>();
        List<byte> tx_buffer = new List<byte>();

        public SerialPort Port { get => port; set => port = value; }

        public Queue<byte> Receiver { get => receiver; set => receiver = value; }
        public Queue<byte> Transfer { get => transfer; set => transfer = value; }
        public List<byte> Tx_buffer { get => tx_buffer; set => tx_buffer = value; }
        public bool Parsing_start { get => parsing_start; set => parsing_start = value; }
        public List<byte> Parse_buffer { get => parse_buffer; set => parse_buffer = value; }
        public uint Rx_time_out_tick { get => rx_time_out_tick; set => rx_time_out_tick = value; }
        public Queue<byte> File_writer { get => file_writer; set => file_writer = value; }
        public List<byte> File_data_bin { get => file_data_bin; set => file_data_bin = value; }
        public List<string> File_data_hex { get => file_data_hex; set => file_data_hex = value; }
        public List<byte> File_data_parsed { get => file_data_parsed; set => file_data_parsed = value; }
        public uint Start_address { get => start_address; set => start_address = value; }

        public void clearParseBuffer()
        {
            Parse_buffer.Clear();
        }

        public void pushParseBuffer(byte data)
        {
            Parse_buffer.Add(data);
        }
        public byte parseOperate()
        {
            byte ret = (byte)Def.CMD_ENUM.CMD_LENGTH;
            byte checksum_buff = parse_buffer[parse_buffer.Count - 1];
            byte checksum_cal = 0;
            tx_buffer.Clear();

            for (int i = 0; i < parse_buffer.Count - 1; i++)
            {
                if (i == 0)
                {
                    checksum_cal = parse_buffer[i];
                }
                else
                {
                    checksum_cal ^= parse_buffer[i];
                }
            }
            if (checksum_cal == checksum_buff)
            {
                switch (parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.CMD_INDEX])
                {
                    case (byte)Def.CMD_ENUM.TX_START:
                        ret = (byte)Def.CMD_ENUM.FILE_WRITE;
                        break;
                    case (byte)Def.CMD_ENUM.FILE_WRITE:
                        if(file_writer.Count == 0)
                        {
                            ret = (byte)Def.CMD_ENUM.TX_END;
                        }
                        else
                        {
                            ret = (byte)Def.CMD_ENUM.FILE_WRITE;
                        }
                        break;
                    case (byte)Def.CMD_ENUM.FILE_READ:
                        ret = (byte)Def.CMD_ENUM.TX_END;
                        break;
                    case (byte)Def.CMD_ENUM.TX_END:
                        int length = parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.DATA_LENGTH];
                        UInt32 tick_cost = 0;
                        tick_cost = (UInt32)(parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.DATA_START] << 24);
                        tick_cost += (UInt32)(parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.DATA_START + 1] << 16);
                        tick_cost += (UInt32)(parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.DATA_START + 2] << 8);
                        tick_cost += (UInt32)(parse_buffer[(int)Def.COMM_PARSE_PROTOCOL_INDEX.DATA_START + 3]);
                        MessageBox.Show("DONE\r\n" + (tick_cost / 1000).ToString() + " sec costed");
                        ret = (byte)Def.CMD_ENUM.CMD_LENGTH;
                        break;
                    case (byte)Def.CMD_ENUM.ERR_CHECKSUM:
                        break;
                    case (byte)Def.CMD_ENUM.ERR_TIMEOUT:
                        break;
                    case (byte)Def.CMD_ENUM.ERR_FLASH_RANGE:
                        break;
                    default:
                        break;
                }
            }
            else
            {
                MessageBox.Show("error checksum");
            }

            return ret;
        }

        public bool parseHexFileDataToBin()
        {
            if(checkHexFileLineChecksum(0) == false)
            {
                MessageBox.Show("CHECKSUM ERR" + " LINE 0");
                return false;
            }
            byte data_length = Convert.ToByte(file_data_hex[0].Substring((int)Def.HEX_STRING_INDEX.LENGTH_START, 2), 16);
            byte type = Convert.ToByte(file_data_hex[0].Substring((int)Def.HEX_STRING_INDEX.TYPE_START, 2), 16);
            start_address = Convert.ToUInt32(file_data_hex[0].Substring((int)Def.HEX_STRING_INDEX.DATA_START, data_length * 2), 16);
            file_data_parsed = new List<byte>();
            if (type == (byte)Def.HEX_TYPE_ENUM.ADDRESS)
            {
                start_address = start_address << 16;
            }
            for(int i = 1; i < file_data_hex.Count; i++)
            {
                if(checkHexFileLineChecksum(i) == false)
                {
                    MessageBox.Show("CHECKSUM ERR" + " LINE " + i.ToString());
                    return false;
                }
                if(i == 1)
                {
                    start_address += Convert.ToUInt32(file_data_hex[i].Substring((int)Def.HEX_STRING_INDEX.ADDRESS_START, 4), 16);
                }
                data_length = Convert.ToByte(file_data_hex[i].Substring((int)Def.HEX_STRING_INDEX.LENGTH_START, 2), 16);
                type = Convert.ToByte(file_data_hex[i].Substring((int)Def.HEX_STRING_INDEX.TYPE_START, 2), 16);
                
                switch(type)
                {
                    case (byte)Def.HEX_TYPE_ENUM.DATA:
                        for(int j = 0; j < data_length; j++)
                        {
                            byte data = Convert.ToByte(file_data_hex[i].Substring((int)Def.HEX_STRING_INDEX.DATA_START + j * 2, 2), 16);
                            file_data_parsed.Add(data);
                        }
                        break;
                    case (byte)Def.HEX_TYPE_ENUM.END:
                        break;
                    case (byte)Def.HEX_TYPE_ENUM.ADDRESS:
                        break;
                    case (byte)Def.HEX_TYPE_ENUM.MDK_ADDRESS:
                        break;
                    default:
                        break;
                }
            }
            return true;
        }

        public bool checkHexFileLineChecksum(int index)
        {
            bool ret = true;
            try
            {
                byte checksum = 0;
                string string_received_checksum = file_data_hex[index].Substring(file_data_hex[index].Length - 1 - 1);
                byte received_checksum = Convert.ToByte(string_received_checksum, 16);
                string data_for_check = file_data_hex[index].Substring((int)Def.HEX_STRING_INDEX.LENGTH_START, (file_data_hex[index].Length - 2 - 1));
                for (int i = 0; i < data_for_check.Length; i += 2)
                {
                    string data = data_for_check.Substring(i, 2);
                    checksum += Convert.ToByte(data, 16);
                }
                checksum ^= 0xFF;
                checksum += 1;

                if (checksum != received_checksum)
                {
                    ret = false;
                }
                return ret;
            }
            catch
            {
                MessageBox.Show("HEX FILE checksum ERROR");
                return ret;
            }
        }
    }
}
