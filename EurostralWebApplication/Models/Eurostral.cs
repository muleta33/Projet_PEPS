using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Wrapper_Pricer;

namespace EurostralWebApplication.Models
{
    public class Eurostral
    {
        public DateTime beginDate = new DateTime(2015, 4, 30);

        public Index[] Indexes { get; set; }
        public double Price { get; set; }

        public Eurostral(Index[] indexes)
        {
            Indexes = indexes;
            Price = 0;
        }

        public double getPrice()
        {
            foreach (Index index in Indexes)
                index.updatePastPrices();

            double currentTime = (DateTime.Now - beginDate).Days / 365.0;
            bool isAtObservationDate = Math.Abs((currentTime * 2) % 1) < 1 / 365;
            int numberOfPastPricesPerIndex = Indexes.ElementAt(0).PastPrices.Count;
            if (!isAtObservationDate)
                numberOfPastPricesPerIndex = numberOfPastPricesPerIndex + 1;

            double[] past = new double[numberOfPastPricesPerIndex * 3];

            int currentUnderlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                int currentPriceIndex = 0;
                foreach (double price in index.PastPrices)
                {
                    past[currentPriceIndex * 3 + currentUnderlyingIndex] = price;
                    currentPriceIndex++;
                }
                currentUnderlyingIndex++;
            }

            currentUnderlyingIndex = 0;
            if (!isAtObservationDate)
            {
                foreach (Index index in Indexes)
                {
                    past[Indexes.ElementAt(0).PastPrices.Count * 3 + currentUnderlyingIndex] = index.getCurrentPrice();
                    currentUnderlyingIndex++;
                }
            }
            
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_price_at(currentTime, past, numberOfPastPricesPerIndex);
            Price = wrapper.get_price();
            return Price;
        }
    }
}