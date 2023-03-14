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
        
        
        List<byte> file_data = new List<byte>();
        Queue<byte> file_writer = new Queue<byte>();

        Queue<byte> transfer = new Queue<byte>();
        Queue<byte> receiver = new Queue<byte>();
        List<byte> parse_buffer = new List<byte>();
        List<byte> tx_buffer = new List<byte>();

        public SerialPort Port { get => port; set => port = value; }

        public Queue<byte> Receiver { get => receiver; set => receiver = value; }
        public List<byte> File_data { get => file_data; set => file_data = value; }
        public Queue<byte> Transfer { get => transfer; set => transfer = value; }
        public List<byte> Tx_buffer { get => tx_buffer; set => tx_buffer = value; }
        public bool Parsing_start { get => parsing_start; set => parsing_start = value; }
        public List<byte> Parse_buffer { get => parse_buffer; set => parse_buffer = value; }
        public uint Rx_time_out_tick { get => rx_time_out_tick; set => rx_time_out_tick = value; }
        public Queue<byte> File_writer { get => file_writer; set => file_writer = value; }

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
            byte ret = (byte)Def.CMD_ENUM.TX_END;
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
                switch (parse_buffer[(int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX])
                {
                    case (byte)Def.CMD_ENUM.TX_START:
                        ret = (byte)Def.CMD_ENUM.FILE_WRITE;
                        break;
                    case (byte)Def.CMD_ENUM.FILE_WRITE:
                        if(File_writer.Count == 0)
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
                        break;
                    case (byte)Def.CMD_ENUM.ERR_CHECKSUM:
                        break;
                    case (byte)Def.CMD_ENUM.ERR_TIMEOUT:
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
    }
}
