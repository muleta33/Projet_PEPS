namespace UI_Test
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
            this.pricing0 = new System.Windows.Forms.Button();
            this.priceTextbox = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.pricingT = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.hedgingT = new System.Windows.Forms.Button();
            this.hedging0 = new System.Windows.Forms.Button();
            this.deltaSP500 = new System.Windows.Forms.Label();
            this.deltaSP200 = new System.Windows.Forms.Label();
            this.deltaES50 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // pricing0
            // 
            this.pricing0.Location = new System.Drawing.Point(238, 19);
            this.pricing0.Name = "pricing0";
            this.pricing0.Size = new System.Drawing.Size(75, 23);
            this.pricing0.TabIndex = 0;
            this.pricing0.Text = "Pricing at 0";
            this.pricing0.UseVisualStyleBackColor = true;
            this.pricing0.Click += new System.EventHandler(this.pricing0_Click);
            // 
            // priceTextbox
            // 
            this.priceTextbox.AutoSize = true;
            this.priceTextbox.Location = new System.Drawing.Point(54, 53);
            this.priceTextbox.Name = "priceTextbox";
            this.priceTextbox.Size = new System.Drawing.Size(31, 13);
            this.priceTextbox.TabIndex = 1;
            this.priceTextbox.Text = "Price";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.pricingT);
            this.groupBox1.Controls.Add(this.pricing0);
            this.groupBox1.Controls.Add(this.priceTextbox);
            this.groupBox1.Location = new System.Drawing.Point(22, 26);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(353, 113);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Pricing";
            // 
            // pricingT
            // 
            this.pricingT.Location = new System.Drawing.Point(238, 73);
            this.pricingT.Name = "pricingT";
            this.pricingT.Size = new System.Drawing.Size(75, 23);
            this.pricingT.TabIndex = 2;
            this.pricingT.Text = "Pricing at t";
            this.pricingT.UseVisualStyleBackColor = true;
            this.pricingT.Click += new System.EventHandler(this.pricingT_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.hedgingT);
            this.groupBox2.Controls.Add(this.hedging0);
            this.groupBox2.Controls.Add(this.deltaSP500);
            this.groupBox2.Controls.Add(this.deltaSP200);
            this.groupBox2.Controls.Add(this.deltaES50);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(22, 180);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(353, 148);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Hedging";
            // 
            // hedgingT
            // 
            this.hedgingT.Location = new System.Drawing.Point(238, 98);
            this.hedgingT.Name = "hedgingT";
            this.hedgingT.Size = new System.Drawing.Size(84, 23);
            this.hedgingT.TabIndex = 7;
            this.hedgingT.Text = "Hedging at t";
            this.hedgingT.UseVisualStyleBackColor = true;
            this.hedgingT.Click += new System.EventHandler(this.hedgingT_Click);
            // 
            // hedging0
            // 
            this.hedging0.Location = new System.Drawing.Point(238, 49);
            this.hedging0.Name = "hedging0";
            this.hedging0.Size = new System.Drawing.Size(84, 23);
            this.hedging0.TabIndex = 6;
            this.hedging0.Text = "Hedging at 0";
            this.hedging0.UseVisualStyleBackColor = true;
            this.hedging0.Click += new System.EventHandler(this.hedging0_Click);
            // 
            // deltaSP500
            // 
            this.deltaSP500.AutoSize = true;
            this.deltaSP500.Location = new System.Drawing.Point(107, 117);
            this.deltaSP500.Name = "deltaSP500";
            this.deltaSP500.Size = new System.Drawing.Size(32, 13);
            this.deltaSP500.TabIndex = 5;
            this.deltaSP500.Text = "Delta";
            // 
            // deltaSP200
            // 
            this.deltaSP200.AutoSize = true;
            this.deltaSP200.Location = new System.Drawing.Point(107, 78);
            this.deltaSP200.Name = "deltaSP200";
            this.deltaSP200.Size = new System.Drawing.Size(32, 13);
            this.deltaSP200.TabIndex = 4;
            this.deltaSP200.Text = "Delta";
            // 
            // deltaES50
            // 
            this.deltaES50.AutoSize = true;
            this.deltaES50.Location = new System.Drawing.Point(107, 37);
            this.deltaES50.Name = "deltaES50";
            this.deltaES50.Size = new System.Drawing.Size(32, 13);
            this.deltaES50.TabIndex = 3;
            this.deltaES50.Text = "Delta";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(18, 117);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "SP500";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "SP ASX200";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "EuroStoxx50";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(402, 352);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button pricing0;
        private System.Windows.Forms.Label priceTextbox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button pricingT;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button hedgingT;
        private System.Windows.Forms.Button hedging0;
        private System.Windows.Forms.Label deltaSP500;
        private System.Windows.Forms.Label deltaSP200;
        private System.Windows.Forms.Label deltaES50;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
    }
}

