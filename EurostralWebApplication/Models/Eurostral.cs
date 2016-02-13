using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Wrapper_Pricer;

namespace EurostralWebApplication.Models
{
    public class Eurostral
    {
        public Index[] Indexes { get; set; }

        public DateTime BeginDate { get; set; }
        public List<DateTime> ObservationDates { get; set; }

        public List<List<double>> PastMatrix { get; set; }

        private const int underlyingNumber = 3;

        public double Price { get; set; }
        public double[] Hedge { get; set; }

        public Eurostral(Index[] indexes)
        {
            Indexes = indexes;
            
            BeginDate = new DateTime(2015, 4, 30);

            ObservationDates = new List<DateTime>();
            ObservationDates.Add(BeginDate);
            ObservationDates.Add(new DateTime(2015, 10, 30));
            ObservationDates.Add(new DateTime(2016, 5, 2));
            ObservationDates.Add(new DateTime(2016, 10, 31));
            ObservationDates.Add(new DateTime(2017, 5, 2));
            ObservationDates.Add(new DateTime(2017, 10, 31));
            ObservationDates.Add(new DateTime(2018, 4, 30));
            ObservationDates.Add(new DateTime(2018, 10, 30));
            ObservationDates.Add(new DateTime(2019, 4, 30));
            ObservationDates.Add(new DateTime(2019, 10, 30));
            ObservationDates.Add(new DateTime(2020, 4, 30));
            ObservationDates.Add(new DateTime(2020, 10, 30));
            ObservationDates.Add(new DateTime(2021, 4, 30));
            ObservationDates.Add(new DateTime(2021, 11, 2));
            ObservationDates.Add(new DateTime(2022, 5, 2));
            ObservationDates.Add(new DateTime(2022, 10, 31));
            ObservationDates.Add(new DateTime(2013, 4, 27));

            PastMatrix = new List<List<double>>(underlyingNumber);
            for (int i = 0; i < underlyingNumber; ++i)
                PastMatrix.Add(new List<double>());
            fillPastMatrix();
            Price = 0;
            Hedge = new double[underlyingNumber];
        }

        private void fillPastMatrix()
        {
            while ((DateTime.Now - ObservationDates.ElementAt(0)).Days > 1)
            {
                int ind = 0;
                foreach (Index index in Indexes)
                {
                    PastMatrix.ElementAt(ind).Add(index.getPastPrice(ObservationDates.ElementAt(0)));
                    ind++;
                }
                ObservationDates.RemoveAt(0);
            }
        }

        private void fillPastArray(List<List<double>> pastMatrix, double[] past)
        {
            int underlyingIndex = 0;
            foreach (List<double> indexPastPrices in pastMatrix)
            {
                int priceIndex = 0;
                foreach (double price in indexPastPrices)
                {
                    past[priceIndex * underlyingNumber + underlyingIndex] = price;
                    priceIndex++;
                }
                underlyingIndex++;
            }
        }

        private void addCurrentPricesToPastArray(int line, double[] past)
        {
            int underlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                past[line * underlyingNumber + underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }
        }

        public double getPrice()
        {
            // On met à jour la matrice des prix du passé
            fillPastMatrix();

            int numberOfPastPricesPerIndex = PastMatrix.ElementAt(0).Count;

            double currentTime = TimeManagement.convertCurrentTimeInDouble(BeginDate);
            bool isAtObsDate = TimeManagement.isAtObservationDate(currentTime);
            // Si on n'est pas à une date de constatation, on aura une ligne de plus : les prix courants
            if (!isAtObsDate)
                numberOfPastPricesPerIndex++;

            // Allocation matrice de prix des indices de taille cohérente
            double[] past = new double[numberOfPastPricesPerIndex * underlyingNumber];

            // Remplissage de la matrice de prix des indices
            fillPastArray(PastMatrix, past);

            // Ajout des prix courants
            addCurrentPricesToPastArray(numberOfPastPricesPerIndex - 1, past);
            
            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_price_at(currentTime, past, numberOfPastPricesPerIndex);
            Price = wrapper.get_price();
            return Price;
        }

        public double[] getHedging()
        {
            // On met à jour la matrice des prix du passé
            fillPastMatrix();

            int numberOfPastPricesPerIndex = PastMatrix.ElementAt(0).Count;

            double currentTime = TimeManagement.convertCurrentTimeInDouble(BeginDate);
            bool isAtObsDate = TimeManagement.isAtObservationDate(currentTime);
            // Si on n'est pas à une date de constatation, on aura une ligne de plus : les prix courants
            if (!isAtObsDate)
                numberOfPastPricesPerIndex++;

            // Allocation matrice de prix des indices de taille cohérente
            double[] past = new double[numberOfPastPricesPerIndex * underlyingNumber];

            // Remplissage de la matrice de prix des indices
            fillPastArray(PastMatrix, past);

            // Ajout des prix courants
            addCurrentPricesToPastArray(numberOfPastPricesPerIndex - 1, past);

            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper();
            wrapper.compute_deltas_at(currentTime, past, numberOfPastPricesPerIndex);
            Hedge = wrapper.get_deltas();
            return Hedge;
        }
    }
}