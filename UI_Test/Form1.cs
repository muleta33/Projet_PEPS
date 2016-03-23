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

        private void pricing0_Click(object sender, EventArgs e)
        {
            double[] past = { 3051, 2049, 5166, 1, 0.88, 0.67, 3053, 2051, 5168, 1, 0.887, 0.671, 3052, 2048, 5165,
                                1, 0.87, 0.672, 3050, 2050, 5166, 1, 0.879, 0.68, 3054, 2049, 5166, 1, 0.86, 0.67 };
            PricerWrapper wrapper = new PricerWrapper(past, 4, 20000);
            double[] spots = {3055, 2048, 5165, 1, 0.85, 0.68};
            wrapper.compute_price(spots);
            double price = wrapper.get_price();
            priceTextbox.Text = price.ToString();
        }

        private void pricingT_Click(object sender, EventArgs e)
        {
            double[] past = { 3051, 2049, 5166, 1, 0.88, 0.67, 3053, 2051, 5168, 1, 0.887, 0.671, 3052, 2048, 5165,
                                1, 0.87, 0.672, 3050, 2050, 5166, 1, 0.879, 0.68, 3054, 2049, 5166, 1, 0.86, 0.67 };
            PricerWrapper wrapper = new PricerWrapper(past, 4, 20000);
            double[] spots = { 3055, 2048, 5165, 1, 0.85, 0.68 };
            wrapper.compute_price_at(0.4, past, spots, 4);
            double price = wrapper.get_price();
            priceTextbox.Text = price.ToString();
        }

        private void hedging0_Click(object sender, EventArgs e)
        {
            PricerWrapper wrapper = new PricerWrapper(null, 10, 6);
            double[] spots = { 100, 200, 150 };
            wrapper.compute_deltas(spots);
            double[] deltas = new double[3];
            deltas = wrapper.get_deltas();
            deltaES50.Text = deltas[0].ToString();
            deltaSP200.Text = deltas[1].ToString();
            deltaSP500.Text = deltas[2].ToString();
        }

        private void hedgingT_Click(object sender, EventArgs e)
        {
            PricerWrapper wrapper = new PricerWrapper(null, 10, 6);
            double[] past = { 100, 200, 150, 110, 190, 155 };
            double[] spots = { 3055, 2048, 5165, 1, 0.85, 0.68 };
            wrapper.compute_deltas_at(0.4, past, spots, 2);
            double[] deltas = new double[3];
            deltas = wrapper.get_deltas();
            deltaES50.Text = deltas[0].ToString();
            deltaSP200.Text = deltas[1].ToString();
            deltaSP500.Text = deltas[2].ToString();
        }
    }
}
