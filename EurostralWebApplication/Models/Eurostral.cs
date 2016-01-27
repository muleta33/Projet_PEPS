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

        public double Price { get; set; }
        public double[] Hedge { get; set; }

        private const int underlyingNumber = 3;

        public Eurostral()
        {
            Price = 0;
            Hedge = new double[underlyingNumber];
        }

        private double convertCurrentTimeInDouble()
        {
            return (DateTime.Now - beginDate).Days / 365.0;
        }

        private bool isAtObservationDate(double currentTime)
        {
            return Math.Abs((currentTime * 2) % 1) < 1 / 365;
        }

        private void fillPastMatrix(Index[] indexes, double[] past, bool isAtObservationDate)
        {
            int currentUnderlyingIndex = 0;
            foreach (Index index in indexes)
            {
                int currentPriceIndex = 0;
                foreach (double price in index.PastPrices)
                {
                    past[currentPriceIndex * underlyingNumber + currentUnderlyingIndex] = price;
                    currentPriceIndex++;
                }
                currentUnderlyingIndex++;
            }

            currentUnderlyingIndex = 0;
            if (!isAtObservationDate)
            {
                foreach (Index index in indexes)
                {
                    past[indexes.ElementAt(0).PastPrices.Count * underlyingNumber + currentUnderlyingIndex] = index.getCurrentPrice();
                    currentUnderlyingIndex++;
                }
            }
        }

        public double getPrice(Index[] indexes)
        {
            // Allocation matrice de prix des indices de taille cohérente
            double currentTime = convertCurrentTimeInDouble();

            bool isAtObsDate = isAtObservationDate(currentTime);
            int numberOfPastPricesPerIndex = indexes.ElementAt(0).PastPrices.Count;
            if (!isAtObsDate)
                numberOfPastPricesPerIndex = numberOfPastPricesPerIndex + 1;

            double[] past = new double[numberOfPastPricesPerIndex * underlyingNumber];
            // NB matrice past reconstruite à chaque fois, stockage ???

            // Remplissage de la matrice de prix des indices
            fillPastMatrix(indexes, past, isAtObsDate);
            
            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_price_at(currentTime, past, numberOfPastPricesPerIndex);
            Price = wrapper.get_price();
            return Price;
        }

        public void getHedging(Index[] indexes)
        {
            // Allocation matrice de prix des indices de taille cohérente
            double currentTime = convertCurrentTimeInDouble();

            bool isAtObsDate = isAtObservationDate(currentTime);
            int numberOfPastPricesPerIndex = indexes.ElementAt(0).PastPrices.Count;
            if (!isAtObsDate)
                numberOfPastPricesPerIndex = numberOfPastPricesPerIndex + 1;

            double[] past = new double[numberOfPastPricesPerIndex * underlyingNumber];

            // Remplissage de la matrice de prix des indices
            fillPastMatrix(indexes, past, isAtObsDate);

            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_deltas_at(currentTime, past, numberOfPastPricesPerIndex);
            Hedge = wrapper.get_deltas();
        }
    }
}