using System;
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
                    comm_inst.File_data = new List<byte>(file_reader.ReadBytes((int)length));   
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
            if(comm_inst.Port.IsOpen == false)
            {
                comm_inst.Port.PortName = comboBox1.Text;
                comm_inst.Port.BaudRate = 115200;
                comm_inst.Port.Parity = Parity.None;
                comm_inst.Port.DataBits = 8;
                comm_inst.Port.StopBits = StopBits.One;
                comm_inst.Port.DataReceived += new SerialDataReceivedEventHandler(dataReceived);
                comm_inst.Port.Open();
                button3.Text = "OPEN";
            }
            if (comm_inst.File_data.Count != 0)
            {
                comm_inst.File_writer.Clear();

                for (int i = 0; i < comm_inst.File_data.Count; i++)
                {
                    comm_inst.File_writer.Enqueue(comm_inst.File_data[i]);
                }
            }
            startTransmit((byte)Def.CMD_ENUM.TX_START);
        }

        void dataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort port_buff = (SerialPort)sender;
            int length = port_buff.BytesToRead;
            byte[] buffer = new byte[length];
            port_buff.Read(buffer, 0, length);

            for(int i = 0; i < length; i++)
            {
                comm_inst.Receiver.Enqueue(buffer[i]);
            }
        }

        void startTransmit(byte cmd)
        {
            if (comm_inst.Port.IsOpen)
            {
                List<byte> buffer = new List<byte>();
                List<byte> tx_buff;

                cmdRetProcedure(list: comm_inst.Tx_buffer, cmd: cmd);
                tx_buff = comm_inst.Tx_buffer;

                foreach (byte data in tx_buff)
                {
                    comm_inst.Transfer.Enqueue(data);
                }
            }
        }
        
        void pushDataOnList(List<byte> list, byte data, ref byte checksum)
        {
            list.Add(data);

            if (data == (byte)Def.PACKET.SPECIAL_CHAR)
            {
                list.Add((byte)PACKET.SPECIAL_CLEAR);
            }

            checksum ^= data;
        }

        void cmdRetProcedure(List<byte> list, byte cmd)
        {
            byte checksum = cmd;

            switch (cmd)
            {
                case (byte)Def.CMD_ENUM.TX_START:
                    {
                        byte[] write_start_address = new byte[4];

                        list.Add((byte)'*');
                        list.Add(0);
                        pushDataOnList(list: list, data: cmd, checksum: ref checksum);
                        pushDataOnList(list: list, data: 0, checksum: ref checksum);
                        pushDataOnList(list: list, data: (byte)write_start_address.Length, checksum: ref checksum);

                        UInt32 address = Convert.ToUInt32(textBox3.Text, 16);

                        write_start_address[0] = (byte)((address & 0xFF000000) >> 24);
                        write_start_address[1] = (byte)((address & 0x00FF0000) >> 16);
                        write_start_address[2] = (byte)((address & 0x0000FF00) >> 8);
                        write_start_address[3] = (byte)((address & 0x000000FF));

                        pushDataOnList(list: list, data: write_start_address[0], checksum: ref checksum);
                        pushDataOnList(list: list, data: write_start_address[1], checksum: ref checksum);
                        pushDataOnList(list: list, data: write_start_address[2], checksum: ref checksum);
                        pushDataOnList(list: list, data: write_start_address[3], checksum: ref checksum);

                        pushDataOnList(list: list, data: checksum, checksum: ref checksum);
                        list.Add((byte)Def.PACKET.SPECIAL_CHAR);
                        list.Add((byte)Def.PACKET.END_CHAR);
                        break;
                    }
                case (byte)Def.CMD_ENUM.FILE_WRITE:
                    byte data_length_msb = (byte)(comm_inst.File_data.Count >> 8);
                    byte data_length_lsb = (byte)comm_inst.File_data.Count;
                    int data_length = comm_inst.File_data.Count;

                    list.Add((byte)'*');
                    list.Add(0);
                    pushDataOnList(list: list, data: cmd, checksum: ref checksum);
                    pushDataOnList(list: list, data: data_length_msb, checksum: ref checksum);
                    pushDataOnList(list: list, data: data_length_lsb, checksum: ref checksum);

                    if(def_inst.Trans_data_length_max < comm_inst.File_writer.Count)
                    {
                        for(int i = 0; i < def_inst.Trans_data_length_max; i++)
                        {
                            pushDataOnList(list: list, data: comm_inst.File_writer.Dequeue(), checksum: ref checksum);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < comm_inst.File_writer.Count; i++)
                        {
                            pushDataOnList(list: list, data: comm_inst.File_writer.Dequeue(), checksum: ref checksum);
                        }
                    }
                    pushDataOnList(list: list, data: checksum, checksum: ref checksum);
                    list.Add((byte)PACKET.SPECIAL_CHAR);
                    list.Add((byte)PACKET.END_CHAR);
                    break;
                case (byte)Def.CMD_ENUM.FILE_READ:
                    list.Clear();
                    break;
                case (byte)Def.CMD_ENUM.TX_END:
                    list.Clear();
                    break;
                case (byte)Def.CMD_ENUM.ERR_CHECKSUM:
                    break;
                case (byte)Def.CMD_ENUM.ERR_TIMEOUT:
                    break;
                default:
                    break;
            }
        }

        void transmitLoop()
        {
            if(comm_inst.Transfer.Count != 0)
            {
                int tx_length = 0;
                byte[] writer = new byte[def_inst.Trans_data_one_time];
                int transfer_count = comm_inst.Transfer.Count;
                if (transfer_count < def_inst.Trans_data_one_time)
                {
                    for (int i = 0; i < transfer_count; i++)
                    {
                        writer[i] = comm_inst.Transfer.Dequeue();
                        tx_length += 1;
                    }
                }
                else
                {
                    for (int i = 0; i < def_inst.Trans_data_one_time; i++)
                    {
                        writer[i] = comm_inst.Transfer.Dequeue();
                        tx_length += 1;
                    }
                }

                comm_inst.Port.Write(writer, 0, tx_length);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if(button3.Text == "OPEN")
            {
                button3.Text = "CLOSE";
            }
            if(comm_inst.Port.IsOpen == true)
            {
                MessageBox.Show("SERIAL PORT IS CLOSED");
                comm_inst.Port.Close();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // receiver timer
            bool special_char_detected = false;

            if(comm_inst.Parsing_start == false)
            {
                comm_inst.Rx_time_out_tick = 0;

                while (comm_inst.Receiver.Count != 0)
                {
                    comm_inst.Parsing_start = true;

                    Application.DoEvents();
                    byte data_buff = comm_inst.Receiver.Dequeue();

                    switch (data_buff)
                    {
                        case (byte)Def.PACKET.SPECIAL_CHAR:
                            special_char_detected = true;
                            break;
                        case (byte)Def.PACKET.START_CHAR:
                            if (special_char_detected == true)
                            {
                                comm_inst.clearParseBuffer();
                            }
                            else
                            {
                                comm_inst.pushParseBuffer(data_buff);
                            }
                            special_char_detected = false;
                            break;
                        case (byte)Def.PACKET.END_CHAR:
                            if (special_char_detected == true)
                            {
                                startTransmit(comm_inst.parseOperate());
                                comm_inst.Parsing_start = false;
                            }
                            else
                            {
                                comm_inst.pushParseBuffer(data_buff);
                            }
                            special_char_detected = false;
                            break;
                        case (byte)Def.PACKET.SPECIAL_CLEAR:
                            if (special_char_detected == true)
                            {
                                comm_inst.pushParseBuffer((byte)Def.PACKET.SPECIAL_CHAR);
                            }
                            else
                            {
                                comm_inst.pushParseBuffer(data_buff);
                            }
                            special_char_detected = false;
                            break;
                        default:
                            comm_inst.pushParseBuffer(data_buff);
                            special_char_detected = false;
                            break;
                    }
                }
            }
            else
            {
                comm_inst.Rx_time_out_tick += 1;
                if(comm_inst.Rx_time_out_tick > 100)
                {
                    comm_inst.Parsing_start = false;
                }
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            // tx_timer
            transmitLoop();
        }
    }
}