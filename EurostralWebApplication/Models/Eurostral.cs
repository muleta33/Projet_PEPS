using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Wrapper_Pricer;

namespace EurostralWebApplication.Models
{
    public class Eurostral
    {
        public double[] Spots { get; set; }
        public double Price { get; set; }

        public Eurostral()
        {
            Spots = new double[3] {100, 200, 150};
        }

        public double getPrice()
        {
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_price(Spots);
            Price = wrapper.get_price();
            return Price;
        }
    }
}