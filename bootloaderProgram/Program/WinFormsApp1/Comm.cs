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
        Queue<byte> transfer = new Queue<byte>();
        SerialPort port = new SerialPort();
        List<byte> data = new List<byte>();

        public Queue<byte> Transfer { get => transfer; set => transfer = value; }
        public SerialPort Port { get => port; set => port = value; }
        public List<byte> Data { get => data; set => data = value; }
    }
}
