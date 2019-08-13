using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace CANSender
{
    public partial class FormMain : Form
    {
        int DataSent = 0;
        int ACKReceived = 0;

        public FormMain()
        {
            InitializeComponent();

            // enumerate ports
            comboBoxPorts.Items.AddRange(SerialPort.GetPortNames());

            if (comboBoxPorts.Items.Count > 0) comboBoxPorts.SelectedIndex = 0;

            comboBoxMode.SelectedIndex = 0;
        }

        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            serialPort1.BaudRate = 3125000;
            serialPort1.PortName = (string)comboBoxPorts.SelectedItem;
            serialPort1.Open();

            serialPort1.DataReceived += SerialPort1_DataReceived;

            buttonOpenPort.Enabled = false;
            buttonSend.Enabled = true;
        }


        // Handle responses/executed commands
        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;

            string rcvStr = sp.ReadExisting();
            if(rcvStr.Count() > 0)
            {
                if(rcvStr[0] == 'A')
                {
                    ACKReceived++;
                }
            }

        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            // fill data
            UInt32 canID = UInt32.Parse(textBoxCANID.Text);
            
            byte[] canData = new byte[8];
            canData[0] = byte.Parse(textBoxDATA0.Text);
            canData[1] = byte.Parse(textBoxDATA1.Text);
            canData[2] = byte.Parse(textBoxDATA2.Text);
            canData[3] = byte.Parse(textBoxDATA3.Text);
            canData[4] = byte.Parse(textBoxDATA4.Text);
            canData[5] = byte.Parse(textBoxDATA5.Text);
            canData[6] = byte.Parse(textBoxDATA6.Text);
            canData[7] = byte.Parse(textBoxDATA7.Text);

            bool isBurst = false;
            if (comboBoxMode.Text == "Burst") isBurst = true;

            byte[] dataToSend = FormatPacket(canID, canData, isBurst);

            serialPort1.Write(dataToSend, 0, dataToSend.Count());
            DataSent++;         
        }


        byte[] FormatPacket(UInt32 canID, byte[] data, bool isBurst )
        {
            byte[] dataToSend = new byte[15];

            dataToSend[0] = (byte)'X';
            dataToSend[1] = isBurst ? (byte)'B' : (byte)'O';

            // ID
            byte[] canIDArray = BitConverter.GetBytes(canID);
            canIDArray.CopyTo(dataToSend, 2);

            // DATA         
            data.CopyTo(dataToSend, 6);

            // End of packet
            dataToSend[14] = (byte)'F';

            return dataToSend;
        }

        private void timerGUI_Tick(object sender, EventArgs e)
        {
            // update GUI
            textBoxSentCommands.Text = DataSent.ToString();
            textBoxExecutedCommands.Text = ACKReceived.ToString();
        }
    }
}
