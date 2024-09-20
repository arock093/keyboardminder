namespace ToolManagerGUI
{
    partial class Form1
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
            this.label1 = new System.Windows.Forms.Label();
            this.toolStatusLabel = new System.Windows.Forms.Label();
            this.startRadioButton = new System.Windows.Forms.RadioButton();
            this.pauseRadioButton = new System.Windows.Forms.RadioButton();
            this.stopRadioButton = new System.Windows.Forms.RadioButton();
            this.restartRadioButton = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.durationTextBox = new System.Windows.Forms.TextBox();
            this.goButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(111, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Status";
            // 
            // toolStatusLabel
            // 
            this.toolStatusLabel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.toolStatusLabel.AutoSize = true;
            this.toolStatusLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStatusLabel.Location = new System.Drawing.Point(33, 58);
            this.toolStatusLabel.Name = "toolStatusLabel";
            this.toolStatusLabel.Size = new System.Drawing.Size(42, 20);
            this.toolStatusLabel.TabIndex = 1;
            this.toolStatusLabel.Text = "label";
            // 
            // startRadioButton
            // 
            this.startRadioButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.startRadioButton.AutoSize = true;
            this.startRadioButton.Location = new System.Drawing.Point(28, 151);
            this.startRadioButton.Name = "startRadioButton";
            this.startRadioButton.Size = new System.Drawing.Size(47, 17);
            this.startRadioButton.TabIndex = 2;
            this.startRadioButton.TabStop = true;
            this.startRadioButton.Text = "Start";
            this.startRadioButton.UseVisualStyleBackColor = true;
            // 
            // pauseRadioButton
            // 
            this.pauseRadioButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pauseRadioButton.AutoSize = true;
            this.pauseRadioButton.Location = new System.Drawing.Point(28, 174);
            this.pauseRadioButton.Name = "pauseRadioButton";
            this.pauseRadioButton.Size = new System.Drawing.Size(55, 17);
            this.pauseRadioButton.TabIndex = 3;
            this.pauseRadioButton.TabStop = true;
            this.pauseRadioButton.Text = "Pause";
            this.pauseRadioButton.UseVisualStyleBackColor = true;
            // 
            // stopRadioButton
            // 
            this.stopRadioButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.stopRadioButton.AutoSize = true;
            this.stopRadioButton.Location = new System.Drawing.Point(28, 197);
            this.stopRadioButton.Name = "stopRadioButton";
            this.stopRadioButton.Size = new System.Drawing.Size(47, 17);
            this.stopRadioButton.TabIndex = 4;
            this.stopRadioButton.TabStop = true;
            this.stopRadioButton.Text = "Stop";
            this.stopRadioButton.UseVisualStyleBackColor = true;
            // 
            // restartRadioButton
            // 
            this.restartRadioButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.restartRadioButton.AutoSize = true;
            this.restartRadioButton.Location = new System.Drawing.Point(28, 220);
            this.restartRadioButton.Name = "restartRadioButton";
            this.restartRadioButton.Size = new System.Drawing.Size(59, 17);
            this.restartRadioButton.TabIndex = 5;
            this.restartRadioButton.TabStop = true;
            this.restartRadioButton.Text = "Restart";
            this.restartRadioButton.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(336, 177);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(60, 16);
            this.label2.TabIndex = 6;
            this.label2.Text = "Duration:";
            // 
            // durationTextBox
            // 
            this.durationTextBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.durationTextBox.Location = new System.Drawing.Point(399, 176);
            this.durationTextBox.Name = "durationTextBox";
            this.durationTextBox.Size = new System.Drawing.Size(100, 20);
            this.durationTextBox.TabIndex = 7;
            // 
            // goButton
            // 
            this.goButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.goButton.Location = new System.Drawing.Point(560, 174);
            this.goButton.Name = "goButton";
            this.goButton.Size = new System.Drawing.Size(75, 23);
            this.goButton.TabIndex = 8;
            this.goButton.Text = "Go";
            this.goButton.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.goButton);
            this.Controls.Add(this.durationTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.restartRadioButton);
            this.Controls.Add(this.stopRadioButton);
            this.Controls.Add(this.pauseRadioButton);
            this.Controls.Add(this.startRadioButton);
            this.Controls.Add(this.toolStatusLabel);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "ToolManager";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label toolStatusLabel;
        private System.Windows.Forms.RadioButton startRadioButton;
        private System.Windows.Forms.RadioButton pauseRadioButton;
        private System.Windows.Forms.RadioButton stopRadioButton;
        private System.Windows.Forms.RadioButton restartRadioButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox durationTextBox;
        private System.Windows.Forms.Button goButton;
    }
}

