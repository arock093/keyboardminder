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
        public Form1()
        {
            InitializeComponent();
            List<string> pnames = new List<string>();
            var lines = File.ReadLines(@"C:\Users\arock\Documents\ToolManagerConfig.txt");
            foreach (var line in lines)
                pnames.Add(line);
            toolStatusLabel.Text = "";
            for (int i = 0; i < pnames.Count; i++)
            {
                Process[] pname = Process.GetProcessesByName(pnames[i]);
                if (pname.Length == 0)
                {

                    toolStatusLabel.Text += pnames[i] + ": Not Running\n";
                }
                else
                {
                    toolStatusLabel.Text += pnames[i] + ": Running\n";
                }
            }
        }
    }
}
