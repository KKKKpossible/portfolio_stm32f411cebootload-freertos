using System.IO.Ports;
using System.Security.Cryptography.Xml;
using System.Text;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        enum cmd
        {
            TX_START, FILE_WRITE, FILE_READ, TX_END, ERR_CHECKSUM, ERR_TIMEOUT
        };

        Queue<byte> transfer = new Queue<byte>();
        SerialPort port = new SerialPort();
        List<byte> data = new List<byte>();
        int transbuff_length_max = 255;
        

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
                    data = new List<byte>(file_reader.ReadBytes((int)length));   
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
            port.PortName = comboBox1.Text;
            port.BaudRate = 115200;
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.DataReceived += new SerialDataReceivedEventHandler(dataReceived);

            port.Open();
            startTransmit();

            port.Close();
        }

        void dataReceived(object sender, SerialDataReceivedEventArgs e)
        {

        }

        void startTransmit()
        {
            transfer = new Queue<byte>();

            data.ForEach(buff =>
            {
                transfer.Enqueue(buff);
            });

            transmitLoop();
        }

        void transmitLoop()
        {
            if(port.IsOpen)
            {
                byte[] buffer = new byte[transbuff_length_max];
                buffer[0] = (byte)'*';
                buffer[1] = 0;

            }
        }
    }
}