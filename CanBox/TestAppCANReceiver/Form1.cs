using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CANReceiver
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private UdpClient udpClient;
        private int ReceiveCounter = 0;

        private void buttonStart_Click(object sender, EventArgs e)
        {
            buttonStart.Enabled = false;

            Task.Run(async () =>
            {
                using (var udpClient = new UdpClient(12345))
                {
                    while (true)
                    {
                        //IPEndPoint object will allow us to read datagrams sent from any source.
                        var receivedResults = await udpClient.ReceiveAsync();
                        ReceiveCounter++;
                        //loggingEvent += Encoding.ASCII.GetString(receivedResults.Buffer);
                    }
                }
            });

        }

        private void timerTicks_Tick(object sender, EventArgs e)
        {
            textBoxReceivedCount.Text = ReceiveCounter.ToString();
        }
    }
}
