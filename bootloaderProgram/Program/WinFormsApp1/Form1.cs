using System.Text;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();
            List<byte> data;

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
    }
}