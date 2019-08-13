namespace CANSender
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.comboBoxPorts = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonOpenPort = new System.Windows.Forms.Button();
            this.buttonSend = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxSentCommands = new System.Windows.Forms.TextBox();
            this.textBoxExecutedCommands = new System.Windows.Forms.TextBox();
            this.timerGUI = new System.Windows.Forms.Timer(this.components);
            this.comboBoxMode = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxCANID = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxDATA7 = new System.Windows.Forms.TextBox();
            this.textBoxDATA6 = new System.Windows.Forms.TextBox();
            this.textBoxDATA5 = new System.Windows.Forms.TextBox();
            this.textBoxDATA4 = new System.Windows.Forms.TextBox();
            this.textBoxDATA3 = new System.Windows.Forms.TextBox();
            this.textBoxDATA2 = new System.Windows.Forms.TextBox();
            this.textBoxDATA1 = new System.Windows.Forms.TextBox();
            this.textBoxDATA0 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBoxPorts
            // 
            this.comboBoxPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPorts.FormattingEnabled = true;
            this.comboBoxPorts.Location = new System.Drawing.Point(114, 30);
            this.comboBoxPorts.Name = "comboBoxPorts";
            this.comboBoxPorts.Size = new System.Drawing.Size(121, 21);
            this.comboBoxPorts.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Serial Port:";
            // 
            // buttonOpenPort
            // 
            this.buttonOpenPort.Location = new System.Drawing.Point(250, 21);
            this.buttonOpenPort.Name = "buttonOpenPort";
            this.buttonOpenPort.Size = new System.Drawing.Size(89, 36);
            this.buttonOpenPort.TabIndex = 2;
            this.buttonOpenPort.Text = "Open";
            this.buttonOpenPort.UseVisualStyleBackColor = true;
            this.buttonOpenPort.Click += new System.EventHandler(this.buttonOpenPort_Click);
            // 
            // buttonSend
            // 
            this.buttonSend.Enabled = false;
            this.buttonSend.Location = new System.Drawing.Point(456, 116);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(98, 47);
            this.buttonSend.TabIndex = 3;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(365, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Sent:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(365, 41);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Executed:";
            // 
            // textBoxSentCommands
            // 
            this.textBoxSentCommands.Location = new System.Drawing.Point(431, 12);
            this.textBoxSentCommands.Name = "textBoxSentCommands";
            this.textBoxSentCommands.ReadOnly = true;
            this.textBoxSentCommands.Size = new System.Drawing.Size(100, 20);
            this.textBoxSentCommands.TabIndex = 6;
            // 
            // textBoxExecutedCommands
            // 
            this.textBoxExecutedCommands.Location = new System.Drawing.Point(431, 38);
            this.textBoxExecutedCommands.Name = "textBoxExecutedCommands";
            this.textBoxExecutedCommands.ReadOnly = true;
            this.textBoxExecutedCommands.Size = new System.Drawing.Size(100, 20);
            this.textBoxExecutedCommands.TabIndex = 7;
            // 
            // timerGUI
            // 
            this.timerGUI.Enabled = true;
            this.timerGUI.Tick += new System.EventHandler(this.timerGUI_Tick);
            // 
            // comboBoxMode
            // 
            this.comboBoxMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxMode.FormattingEnabled = true;
            this.comboBoxMode.Items.AddRange(new object[] {
            "Single",
            "Burst"});
            this.comboBoxMode.Location = new System.Drawing.Point(58, 27);
            this.comboBoxMode.Name = "comboBoxMode";
            this.comboBoxMode.Size = new System.Drawing.Size(121, 21);
            this.comboBoxMode.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 30);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Mode:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(17, 57);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(21, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "ID:";
            // 
            // textBoxCANID
            // 
            this.textBoxCANID.Location = new System.Drawing.Point(58, 54);
            this.textBoxCANID.Name = "textBoxCANID";
            this.textBoxCANID.Size = new System.Drawing.Size(121, 20);
            this.textBoxCANID.TabIndex = 11;
            this.textBoxCANID.Text = "1000";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxDATA7);
            this.groupBox1.Controls.Add(this.textBoxDATA6);
            this.groupBox1.Controls.Add(this.textBoxDATA5);
            this.groupBox1.Controls.Add(this.textBoxDATA4);
            this.groupBox1.Controls.Add(this.textBoxDATA3);
            this.groupBox1.Controls.Add(this.textBoxDATA2);
            this.groupBox1.Controls.Add(this.textBoxDATA1);
            this.groupBox1.Controls.Add(this.textBoxDATA0);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.comboBoxMode);
            this.groupBox1.Controls.Add(this.textBoxCANID);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Location = new System.Drawing.Point(25, 76);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(416, 130);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "CAN";
            // 
            // textBoxDATA7
            // 
            this.textBoxDATA7.Location = new System.Drawing.Point(351, 85);
            this.textBoxDATA7.Name = "textBoxDATA7";
            this.textBoxDATA7.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA7.TabIndex = 20;
            this.textBoxDATA7.Text = "0";
            // 
            // textBoxDATA6
            // 
            this.textBoxDATA6.Location = new System.Drawing.Point(309, 85);
            this.textBoxDATA6.Name = "textBoxDATA6";
            this.textBoxDATA6.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA6.TabIndex = 19;
            this.textBoxDATA6.Text = "0";
            // 
            // textBoxDATA5
            // 
            this.textBoxDATA5.Location = new System.Drawing.Point(267, 85);
            this.textBoxDATA5.Name = "textBoxDATA5";
            this.textBoxDATA5.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA5.TabIndex = 18;
            this.textBoxDATA5.Text = "0";
            // 
            // textBoxDATA4
            // 
            this.textBoxDATA4.Location = new System.Drawing.Point(225, 85);
            this.textBoxDATA4.Name = "textBoxDATA4";
            this.textBoxDATA4.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA4.TabIndex = 17;
            this.textBoxDATA4.Text = "0";
            // 
            // textBoxDATA3
            // 
            this.textBoxDATA3.Location = new System.Drawing.Point(184, 85);
            this.textBoxDATA3.Name = "textBoxDATA3";
            this.textBoxDATA3.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA3.TabIndex = 16;
            this.textBoxDATA3.Text = "78";
            // 
            // textBoxDATA2
            // 
            this.textBoxDATA2.Location = new System.Drawing.Point(142, 85);
            this.textBoxDATA2.Name = "textBoxDATA2";
            this.textBoxDATA2.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA2.TabIndex = 15;
            this.textBoxDATA2.Text = "56";
            // 
            // textBoxDATA1
            // 
            this.textBoxDATA1.Location = new System.Drawing.Point(100, 85);
            this.textBoxDATA1.Name = "textBoxDATA1";
            this.textBoxDATA1.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA1.TabIndex = 14;
            this.textBoxDATA1.Text = "34";
            // 
            // textBoxDATA0
            // 
            this.textBoxDATA0.Location = new System.Drawing.Point(58, 85);
            this.textBoxDATA0.Name = "textBoxDATA0";
            this.textBoxDATA0.Size = new System.Drawing.Size(36, 20);
            this.textBoxDATA0.TabIndex = 13;
            this.textBoxDATA0.Text = "12";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(17, 88);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(33, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Data:";
            // 
            // FormMain
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(570, 216);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxExecutedCommands);
            this.Controls.Add(this.textBoxSentCommands);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.buttonOpenPort);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBoxPorts);
            this.Name = "FormMain";
            this.Text = "CAN Sender";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox comboBoxPorts;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonOpenPort;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxSentCommands;
        private System.Windows.Forms.TextBox textBoxExecutedCommands;
        private System.Windows.Forms.Timer timerGUI;
        private System.Windows.Forms.ComboBox comboBoxMode;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxCANID;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBoxDATA7;
        private System.Windows.Forms.TextBox textBoxDATA6;
        private System.Windows.Forms.TextBox textBoxDATA5;
        private System.Windows.Forms.TextBox textBoxDATA4;
        private System.Windows.Forms.TextBox textBoxDATA3;
        private System.Windows.Forms.TextBox textBoxDATA2;
        private System.Windows.Forms.TextBox textBoxDATA1;
        private System.Windows.Forms.TextBox textBoxDATA0;
        private System.Windows.Forms.Label label6;
    }
}

