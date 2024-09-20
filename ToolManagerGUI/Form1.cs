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
using System.Windows.Forms;

namespace ToolManagerGUI
{
    public partial class Form1 : Form
    {
        List<CheckBox> checkBoxes = new List<CheckBox>();
        int[] running;
        public Form1()
        {
            InitializeComponent();

            startRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            pauseRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            stopRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);
            restartRadioButton.CheckedChanged += new EventHandler(radioButtons_CheckedChanged);

            List<string> pnames = new List<string>();
            var lines = File.ReadLines("ToolManagerConfig.txt");
            foreach (var line in lines)
                pnames.Add(line);
            running = new int[pnames.Count];
            toolStatusLabel.Text = "";
            int y = 154;
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

                Process[] pname = Process.GetProcessesByName(pnames[i]);
                if (pname.Length == 0)
                {

                    toolStatusLabel.Text += pnames[i] + ": Not Running\n";
                }
                else
                {
                    toolStatusLabel.Text += pnames[i] + ": Running\n";
                    running[i] = 1;
                }
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
    }
}
