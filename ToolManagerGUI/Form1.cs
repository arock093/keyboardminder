using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;
using System.Xml.XPath;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace ToolManagerGUI
{
    public partial class Form1 : Form
    {
        List<CheckBox> checkBoxes = new List<CheckBox>();
        List<string> pnames = new List<string>();
        int[] running;
        static System.Timers.Timer aTimer = new System.Timers.Timer();
        public Form1()
        {
            InitializeComponent();
            startRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            pauseRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            stopRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            restartRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            durationTextBox.TextChanged += new EventHandler(durationTextBox_TextChanged);

            var lines = File.ReadLines("ToolManagerConfig.txt");
            foreach (var line in lines)
                pnames.Add(line);
            running = new int[pnames.Count];
            toolStatusLabel.Text = "";
            int y = 154;
            int amountRunning = 0;
            for (int i = 0; i < pnames.Count; i++)
            {
                CheckBox checkBox1 = new CheckBox();
                checkBox1.AutoSize = true;
                checkBox1.Location = new System.Drawing.Point(154, y);
                checkBox1.Name = "checkBox1";
                checkBox1.Size = new System.Drawing.Size(80, 17);
                checkBox1.TabIndex = 0;
                checkBox1.Anchor = AnchorStyles.None;
                checkBox1.Text = pnames[i];
                checkBox1.Enabled = false;
                checkBox1.UseVisualStyleBackColor = true;
                Controls.Add(checkBox1);
                checkBoxes.Add(checkBox1);
                y += 20;
                if (pnames[i] == "DisableTaskbar")
                    pnames[i] = "DisableTaskbar-x64";
            }
            amountRunning = checkIfRunning();
            if (amountRunning == 0)
            {
                stopRadioButton.Enabled = false;
                pauseRadioButton.Enabled = false;
                restartRadioButton.Enabled = false;
            }
        }
        private void radioButtons_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton rb = sender as RadioButton;
            if (rb != null)
            {
                if (rb.Checked)
                {
                    for (int i = 0; i < checkBoxes.Count; i++)
                    {
                        checkBoxes[i].Checked = false;
                        if (rb.Name == "startRadioButton")
                        {
                            if (running[i] == 1)
                                checkBoxes[i].Enabled = false;
                            else
                                checkBoxes[i].Enabled = true;
                        }
                        else
                        {
                            if (running[i] == 1)
                                checkBoxes[i].Enabled = true;
                            else
                                checkBoxes[i].Enabled = false;
                        }
                        if (rb.Name == "stopRadioButton" || rb.Name == "restartRadioButton")
                        {
                            durationTextBox.Enabled = false;
                        }
                        else
                        {
                            durationTextBox.Enabled = true;
                        }
                    }
                }
            }
        }

        private int checkIfRunning()
        {
            toolStatusLabel.Text = "";
            int amountRunning = 0;
            for (int i = 0; i < pnames.Count; i++)
            {
                Process[] pname = Process.GetProcessesByName(pnames[i]);
                if (pname.Length == 0)
                {
                    toolStatusLabel.Text += pnames[i] + ": Not Running\n";
                    running[i] = 0;
                }
                else
                {
                    toolStatusLabel.Text += pnames[i] + ": Running\n";
                    running[i] = 1;
                    amountRunning++;
                }
            }
            return amountRunning;
        }

        private void OnTimedEvent(object source, ElapsedEventArgs e)
        {
            aTimer.Stop();
            if (this.InvokeRequired)
            {
                this.Invoke(new Action(() => UpdateControl()));
            }
            else
            {
                UpdateControl();
            }    
        }

        private void UpdateControl()
        {
            int amountRunning = 0;
            amountRunning = checkIfRunning();
            if (amountRunning == 0)
            {
                startRadioButton.Enabled = true;
                stopRadioButton.Enabled = false;
                pauseRadioButton.Enabled = false;
                restartRadioButton.Enabled = false;
            }
            else if (amountRunning == 3)
            {
                startRadioButton.Enabled = false;
                stopRadioButton.Enabled = true;
                pauseRadioButton.Enabled = true;
                restartRadioButton.Enabled = true;
            }
            else
            {
                startRadioButton.Enabled = true;
                stopRadioButton.Enabled = true;
                pauseRadioButton.Enabled = true;
                restartRadioButton.Enabled = true;
            }
            if (startRadioButton.Checked)
            {
                startRadioButton.Checked = false;
                startRadioButton.Checked = true;
            }
            else if (pauseRadioButton.Checked)
            {
                pauseRadioButton.Checked = false;
                pauseRadioButton.Checked = true;
            }
            else if (stopRadioButton.Checked)
            {
                stopRadioButton.Checked = false;
                stopRadioButton.Checked = true;
            }
            else if (restartRadioButton.Checked)
            {
                restartRadioButton.Checked = false;
                restartRadioButton.Checked = true;
            }
        }

        private void durationTextBox_TextChanged(object sender, EventArgs e)
        {
            if (int.TryParse(durationTextBox.Text, out int value))
            {
                durationTextBox.ForeColor = Color.Black;
            }
            else
            {
                durationTextBox.ForeColor = Color.Red;
            }
        }

        private void goButton_Click(object sender, EventArgs e)
        {
            durationTextBox.ForeColor = Color.Black;
            Process process = new Process();
            process.StartInfo.FileName = "ToolManager.exe";
            process.StartInfo.Arguments = "";
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.CreateNoWindow = true;

            if (startRadioButton.Checked)
            {
                process.StartInfo.Arguments += "start ";
            }
            else if (stopRadioButton.Checked)
            {
                process.StartInfo.Arguments += "stop ";
            }
            else if (pauseRadioButton.Checked)
            {
                process.StartInfo.Arguments += "pause ";
            }
            else if (restartRadioButton.Checked)
            {
                process.StartInfo.Arguments += "restart ";
            }
            for (int i = 0; i < checkBoxes.Count; i++)
            {
                if (checkBoxes[i].Checked)
                {
                    process.StartInfo.Arguments += checkBoxes[i].Text + " ";
                }
            }
            if (durationTextBox.Text != "")
            {
                if (int.TryParse(durationTextBox.Text, out int value))
                {
                    process.StartInfo.Arguments += "-d " + durationTextBox.Text;
                    process.Start();
                    messageLabel.Text = "Success";
                    process.WaitForExit();
                    durationTextBox.Text = "";
                    aTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
                    aTimer.Interval = value * 60 * 1000 + 3000;
                    aTimer.Enabled = true;
                }
                else
                {
                    durationTextBox.ForeColor = Color.Red;
                    messageLabel.Text = "Duration needs to be a number.";
                }
            }
            else
            {
                process.Start();
                messageLabel.Text = "Success";
                process.WaitForExit();
            }
            toolStatusLabel.Text = "";
            int amountRunning = 0;
            amountRunning = checkIfRunning();
            if (amountRunning == 0)
            {
                startRadioButton.Enabled = true;
                stopRadioButton.Enabled = false;
                pauseRadioButton.Enabled = false;
                restartRadioButton.Enabled = false;
            }
            else if (amountRunning == 3)
            {
                startRadioButton.Enabled = false;
                stopRadioButton.Enabled = true;
                pauseRadioButton.Enabled = true;
                restartRadioButton.Enabled = true;
            }
            else
            {
                startRadioButton.Enabled = true;
                stopRadioButton.Enabled = true;
                pauseRadioButton.Enabled = true;
                restartRadioButton.Enabled = true;
            }

            if (startRadioButton.Checked)
            {
                startRadioButton.Checked = false;
                startRadioButton.Checked = true;
            }
            else if (pauseRadioButton.Checked)
            {
                pauseRadioButton.Checked = false;
                pauseRadioButton.Checked = true;
            }
            else if (stopRadioButton.Checked)
            {
                stopRadioButton.Checked = false;
                stopRadioButton.Checked = true;
            }
            else if (restartRadioButton.Checked)
            {
                restartRadioButton.Checked = false;
                restartRadioButton.Checked = true;
            }

            /*
            startRadioButton.Checked = false;
            stopRadioButton.Checked = false;
            pauseRadioButton.Checked = false;
            restartRadioButton.Checked = false;
            for (int i = 0; i < checkBoxes.Count; i++)
            {
                checkBoxes[i].Checked = false;
            }
            */
        }
    }
}
