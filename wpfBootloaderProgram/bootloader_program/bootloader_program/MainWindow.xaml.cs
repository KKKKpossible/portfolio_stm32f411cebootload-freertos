using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace bootloader_program
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        BackgroundWorker? worker;
        
        int sum_out;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }

        protected override void OnInitialized(EventArgs e)
        {
            base.OnInitialized(e);
            sum_out = 0;

            worker = new BackgroundWorker()
            {
                // 중간중간 확인할 수 있게 progress changed 이벤트 발생하도록한다
                WorkerReportsProgress = true,
                // 작업을 취소 가능하도록 한다.
                WorkerSupportsCancellation = true
            };

            worker.DoWork += backDoWork;
            worker.ProgressChanged += backProgressChanged;
            worker.RunWorkerCompleted += backRunWorkerCompleted;

            worker.RunWorkerAsync(10);

        }

        private void backDoWork(object? sender, DoWorkEventArgs e)
        {
            if (e.Argument != null && worker != null)
            {
                int data = (int)e.Argument;
                for (int i = 0; i < data; i++)
                {
                    if (worker.CancellationPending == true)
                    {
                        e.Cancel = true;

                        return;
                    }
                    else
                    {
                        Thread.Sleep(100);
                        this.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                        {
                            if (i % 2 == 0)
                            {
                                sum_out += i;
                                e.Result = sum_out;
                            }
                        });
                    }
                    worker.ReportProgress(i);
                }
            }
        }

        private void backProgressChanged(object? sender, ProgressChangedEventArgs e)
        {
            int data = e.ProgressPercentage;
        }

        private void backRunWorkerCompleted(object? sender, RunWorkerCompletedEventArgs e)
        {
            if(e.Cancelled == true)
            {

            }
            else if(e.Error != null)
            {

            }
            else  // 결과값 동작
            {

            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            OpenFileDialog open_file = new OpenFileDialog();
            if(open_file.ShowDialog() == true)
            {
                textbox_file_path.Text = open_file.FileName;
            }
        }

        private void combo_port_DropDownOpened(object sender, EventArgs e)
        {
        }
    }
}
