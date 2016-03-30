using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Wrapper_Pricer;

namespace EurostralWebApplication.Models
{
    public class Eurostral
    {
        private int NumberOfEstimationDates = 10;

        private int UnderlyingNumber;

        public Index[] Indexes { get; set; }

        public ExchangeRate[] ExchangeRates { get; set; }

        public DateTime BeginDate { get; set; }
        public List<DateTime> ObservationDates { get; set; }

        public double[] PastMarketData { get; set; }
        public List<List<double>> PastMatrix { get; set; }

        public double Price { get; set; }
        public double ConfidenceIntervalLowerBoundary { get; set; }
        public double ConfidenceIntervalUpperBoundary { get; set; }
        public double[] Hedge { get; set; }

        public Eurostral(Index[] indexes, ExchangeRate[] exchangeRates, int underlyingNumber, bool initializePastMatrix = false)
        {
            Indexes = indexes;
            UnderlyingNumber = underlyingNumber;

            ExchangeRates = exchangeRates;

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

            if ((HttpContext.Current.Session["PastMarketData"] == null) || initializePastMatrix)
            {
                PastMarketData = new double[2 * underlyingNumber * NumberOfEstimationDates];
                fillPastMarketData();
                HttpContext.Current.Session["PastMarketData"] = PastMarketData;
            }
            else
                PastMarketData = (double[])HttpContext.Current.Session["PastMarketData"];

            Price = 0;
            ConfidenceIntervalLowerBoundary = 0;
            ConfidenceIntervalUpperBoundary = 0;
            Hedge = new double[2 * underlyingNumber];
        }

        private void fillPastMatrix()
        {
            while ((DateTime.Now - ObservationDates.ElementAt(0)).Days > 1)
            {
                int ind = 0;
                foreach (Index index in Indexes)
                {
                    double pastPrice = 0;
                    DateTime pastDate = ObservationDates.ElementAt(0);
                    while (pastPrice == 0)
                    {
                        pastPrice = index.getPastPrice(pastDate);
                        pastDate = pastDate.AddDays(1);
                    }
                    PastMatrix.ElementAt(ind).Add(pastPrice);
                    ind++;
                }
                ObservationDates.RemoveAt(0);
            }
        }

        private void fillPastMarketData()
        {
            DateTime endEstimationDate = BeginDate.AddDays(-1);
            int ind = 0;
            foreach (Index index in Indexes)
            {
                DateTime currentEstimationDate = endEstimationDate;

                for (int i = NumberOfEstimationDates - 1; i >= 0; --i)
                {
                    double pastPrice = 0;
                    while (pastPrice == 0)
                    {
                        pastPrice = index.getPastPrice(currentEstimationDate);
                        currentEstimationDate = currentEstimationDate.AddDays(-1);
                    }
                    PastMarketData[i * 2 * UnderlyingNumber + ind] = pastPrice;
                }
                ++ind;
            }
            foreach (ExchangeRate exchangeRate in ExchangeRates)
            {
                DateTime currentEstimationDate = endEstimationDate;

                for (int i = NumberOfEstimationDates - 1; i >= 0; --i)
                {
                    double pastValue = 0;
                    while (pastValue == 0)
                    {
                        pastValue = exchangeRate.getPastValue(currentEstimationDate);
                        currentEstimationDate = currentEstimationDate.AddDays(-1);
                    }
                    PastMarketData[i * 2 * UnderlyingNumber + ind] = pastValue;
                }
                ++ind;
            }


            // TODO : Même chose pour les taux de change
            // Suite temporaire

            /*Random randNum = new Random();
 
            for (int i = UnderlyingNumber; i < 2 * UnderlyingNumber; ++i)
            {
                for (int j = 0; j < NumberOfEstimationDates; ++j)
                {
                    if (i == UnderlyingNumber) {
                        PastMarketData[j * 2 * UnderlyingNumber + i] = 1 ;
                    } else if (i == UnderlyingNumber + 1) {
                        PastMarketData[j * 2 * UnderlyingNumber + i] = 0.67 + (randNum.NextDouble() - randNum.NextDouble())*0.01;
                    } else {
                        PastMarketData[j * 2 * UnderlyingNumber + i] = 0.88 + (randNum.NextDouble() - randNum.NextDouble())*0.01;
                }
            
            }*/
        }

        private void fillPastArray(List<List<double>> pastMatrix, double[] past)
        {
            int underlyingIndex = 0;
            foreach (List<double> indexPastPrices in pastMatrix)
            {
                int priceIndex = 0;
                foreach (double price in indexPastPrices)
                {
                    past[priceIndex * UnderlyingNumber + underlyingIndex] = price;
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
                past[line * UnderlyingNumber + underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }
        }

        public void computeCurrentPerformances()
        {
            int ind = 0;
            foreach (Index index in Indexes)
            {
                index.getPerformance(PastMatrix.ElementAt(ind).ElementAt(0));
                ++ind;
            }
        }

        public double getIndexWeight(int ind)
        {
            double[] currentIndexPerformances = new double[UnderlyingNumber];
            int iterationIndex = 0;
            foreach (Index index in Indexes)
            {
                currentIndexPerformances[iterationIndex] = index.Performance;
                ++iterationIndex;
            }

            int[] idx = currentIndexPerformances.Select((x, i) => new KeyValuePair<double, int>(x, i))
                                                  .OrderBy(x => x.Key)
                                                  .Select(x => x.Value)
                                                  .ToArray();
            double weight = 20;
            if (idx[1] == ind)
                weight = 30;
            else if (idx[2] == ind)
                weight = 50;
            return weight;
        }

        public double[] getIndexAndExchangeRateSpots()
        {
            double[] spots = new double[2 * UnderlyingNumber];
            int ind = 0;
            foreach (Index index in Indexes)
            {
                spots[ind] = index.getCurrentPrice();
                ind++;
            }
            foreach (ExchangeRate exchangeRate in ExchangeRates)
            {
                spots[ind] = exchangeRate.getCurrentValue();
                ind++;
            }
            return spots;
        }

        public double getPrice(int numberOfMonteCarloIterations)
        {
            // On met à jour la matrice des prix du passé
            fillPastMatrix();

            int numberOfPastPricesPerIndex = PastMatrix.ElementAt(0).Count;

            double currentTime = TimeManagement.convertCurrentTimeInDouble(BeginDate);
            bool isAtObsDate = TimeManagement.isAtObservationDate(currentTime);

            // Allocation matrice de prix des indices de taille cohérente
            double[] past = new double[numberOfPastPricesPerIndex * UnderlyingNumber];

            // Remplissage de la matrice de prix des indices
            fillPastArray(PastMatrix, past);

            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper(PastMarketData, NumberOfEstimationDates, numberOfMonteCarloIterations);
            wrapper.compute_price_at(currentTime, past, getIndexAndExchangeRateSpots(), numberOfPastPricesPerIndex);
            Price = wrapper.get_price();
            double halfWidthConfidenceInterval = wrapper.get_price_ic();
            ConfidenceIntervalLowerBoundary = Price - halfWidthConfidenceInterval;
            ConfidenceIntervalUpperBoundary = Price + halfWidthConfidenceInterval;
            return Price;
        }

        public double[] getHedging(int numberOfMonteCarloIterations)
        {
            // On met à jour la matrice des prix du passé
            fillPastMatrix();

            int numberOfPastPricesPerIndex = PastMatrix.ElementAt(0).Count;

            double currentTime = TimeManagement.convertCurrentTimeInDouble(BeginDate);
            bool isAtObsDate = TimeManagement.isAtObservationDate(currentTime);

            // Allocation matrice de prix des indices de taille cohérente
            double[] past = new double[numberOfPastPricesPerIndex * UnderlyingNumber];

            // Remplissage de la matrice de prix des indices
            fillPastArray(PastMatrix, past);

            // Appel au pricer
            PricerWrapper wrapper = new PricerWrapper(PastMarketData, NumberOfEstimationDates, numberOfMonteCarloIterations);
            wrapper.compute_deltas_at(currentTime, past, getIndexAndExchangeRateSpots(), numberOfPastPricesPerIndex);
            Hedge = wrapper.get_deltas();
            return Hedge;
        }
    }
}