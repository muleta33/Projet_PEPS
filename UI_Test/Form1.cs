using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Wrapper_Pricer;

namespace UI_Test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.get_price_product(100, 200, 150);
            double price = wrapper.get_price();
            label1.Text = price.ToString();
        }
    }
}
