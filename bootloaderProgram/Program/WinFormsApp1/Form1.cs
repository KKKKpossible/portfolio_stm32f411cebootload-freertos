using System.IO.Ports;
using System.Security.Cryptography.Xml;
using System.Text;
using static WinFormsApp1.Def;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        Def def_inst = new Def();
        Comm comm_inst = new Comm();

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if(result == DialogResult.OK)
            {
                string file_name = openFileDialog1.FileName;
                string full_file_name = openFileDialog1.SafeFileName;

                textBox1.Text = file_name;

                var file_stream = new FileStream(file_name, FileMode.Open);
                
                using(BinaryReader file_reader = new BinaryReader(file_stream))
                {
                    long length = file_reader.BaseStream.Length;
                    comm_inst.Data = new List<byte>(file_reader.ReadBytes((int)length));   
                }
            }
            else
            {
                textBox1.Text = "";
            }
        }

        private void comboBox1_Click(object sender, EventArgs e)
        {
            comboBox1.DataSource = SerialPort.GetPortNames();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            comm_inst.Port.PortName = comboBox1.Text;
            comm_inst.Port.BaudRate = 115200;
            comm_inst.Port.Parity = Parity.None;
            comm_inst.Port.DataBits = 8;
            comm_inst.Port.StopBits = StopBits.One;
            comm_inst.Port.DataReceived += new SerialDataReceivedEventHandler(dataReceived);

            comm_inst.Port.Open();
            startTransmit();

            comm_inst.Port.Close();
        }

        void dataReceived(object sender, SerialDataReceivedEventArgs e)
        {

        }

        void startTransmit()
        {
            comm_inst.Transfer = new Queue<byte>();

            comm_inst.Data.ForEach(buff =>
            {
                comm_inst.Transfer.Enqueue(buff);
            });

            transmitLoop(cmd: (byte)Def.CMD_ENUM.TX_START);
        }

        void cmdRetProcedure(ref byte[] buffer, byte cmd, bool tx_mode, ref int ret_buffer_length)
        {
            if(tx_mode == true)
            {
                byte checksum = cmd;
                ret_buffer_length = 8;

                switch (cmd)
                {
                    case (byte)Def.CMD_ENUM.TX_START:
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX] = cmd;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_MSB_INDEX] = 0;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_LSB_INDEX] = 0;
                        
                        for(int i = (int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX + 1; i < (int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX; i++)
                        {
                            checksum ^= buffer[i];
                        }
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX] = checksum;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_SPECIAL_INDEX] = (byte)Def.PACKET.SPECIAL_CHAR;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_INDEX] = (byte)Def.PACKET.END_CHAR;
                        break;
                    case (byte)Def.CMD_ENUM.FILE_WRITE:
                        byte data_length_msb = (byte)(comm_inst.Transfer.Count >> 8);
                        byte data_length_lsb = (byte)comm_inst.Transfer.Count;
                        int data_length = comm_inst.Transfer.Count;

                        buffer[(int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX] = cmd;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_MSB_INDEX] = data_length_msb;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_LSB_INDEX] = data_length_lsb;
                        int buffer_length = (int)Def.COMM_PROTOCOL_INDEX.DATA_L_LSB_INDEX + 1;
                        for (int i = 0; i < data_length; i++)
                        {
                            int index = i + (int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX;
                            if(index >= def_inst.Transbuff_length_max - 3)
                            {
                                break;
                            }
                            buffer[index] = comm_inst.Transfer.Dequeue();
                            if (buffer[index] == (byte)Def.PACKET.SPECIAL_CHAR)
                            {
                                index += 1;
                                i += 1;
                                buffer_length += 1;
                                buffer[index] = (byte)Def.PACKET.SPECIAL_CLEAR;
                            }
                            buffer_length += 1;
                        }
                        for (int i = (int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX + 1; i < buffer_length - 3; i++)
                        {
                            checksum ^= buffer[i];
                        }
                        ret_buffer_length = buffer_length;
                        buffer[buffer_length - 3] = checksum;
                        buffer[buffer_length - 2] = (byte)Def.PACKET.SPECIAL_CHAR;
                        buffer[buffer_length - 1] = (byte)Def.PACKET.END_CHAR;
                        break;
                    case (byte)Def.CMD_ENUM.FILE_READ:
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX] = cmd;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_MSB_INDEX] = 0;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_LSB_INDEX] = 0;

                        for (int i = (int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX + 1; i < (int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX; i++)
                        {
                            checksum ^= buffer[i];
                        }
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX] = checksum;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_SPECIAL_INDEX] = (byte)Def.PACKET.SPECIAL_CHAR;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_INDEX] = (byte)Def.PACKET.END_CHAR;
                        break;
                    case (byte)Def.CMD_ENUM.TX_END:
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX] = cmd;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_MSB_INDEX] = 0;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_L_LSB_INDEX] = 0;

                        for (int i = (int)Def.COMM_PROTOCOL_INDEX.CMD_INDEX + 1; i < (int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX; i++)
                        {
                            checksum ^= buffer[i];
                        }
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.DATA_START_ONLYCMDCHECKSUM_INDEX] = checksum;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_SPECIAL_INDEX] = (byte)Def.PACKET.SPECIAL_CHAR;
                        buffer[(int)Def.COMM_PROTOCOL_INDEX.ONLYCMD_END_INDEX] = (byte)Def.PACKET.END_CHAR;
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
                switch (cmd)
                {
                    case (byte)Def.CMD_ENUM.TX_START:
                        transmitLoop(cmd: (byte)Def.CMD_ENUM.FILE_WRITE);
                        break;
                    case (byte)Def.CMD_ENUM.FILE_WRITE:
                        transmitLoop(cmd: (byte)Def.CMD_ENUM.TX_END);
                        break;
                    case (byte)Def.CMD_ENUM.FILE_READ:
                        break;
                    case (byte)Def.CMD_ENUM.TX_END:
                        break;
                    case (byte)Def.CMD_ENUM.ERR_CHECKSUM:
                        MessageBox.Show("ERR_CHECKSUM");
                        break;
                    case (byte)Def.CMD_ENUM.ERR_TIMEOUT:
                        MessageBox.Show("ERR_TIMEOUT");
                        break;
                    default:
                        break;
                }
            }
        }

        void transmitLoop(byte cmd)
        {
            if(comm_inst.Port.IsOpen)
            {
                int tx_buffer_length = 0;
                byte[] buffer = new byte[def_inst.Transbuff_length_max];
                buffer[(int)Def.COMM_PROTOCOL_INDEX.START_SPECIAL_INDEX] = (byte)'*';
                buffer[(int)Def.COMM_PROTOCOL_INDEX.START_INDEX] = 0;
                cmdRetProcedure(buffer: ref buffer, cmd: cmd, tx_mode: true, ret_buffer_length: ref tx_buffer_length);

                comm_inst.Port.Write(buffer: buffer, offset: 0, count: tx_buffer_length);
            }
        }
    }
}