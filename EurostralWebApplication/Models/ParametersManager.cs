using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class ParametersManager
    {
        private int NumberOfEstimationDates = 10;

        public ParametersManager(Index[] indexes, int underlyingNumber)
        {
            double[][] historicalPrices = new double[underlyingNumber][];
            double[][] historicalRentabilities = new double[underlyingNumber][];
            for (int i = 0; i < underlyingNumber; ++i)
            {
                historicalPrices[i] = new double[NumberOfEstimationDates + 1];
                historicalRentabilities[i] = new double[NumberOfEstimationDates];
            }
            getHistoricalPrices(indexes, NumberOfEstimationDates, historicalPrices);
            transformPricesIntoRentabilities(historicalPrices, historicalRentabilities);

            ParametersEstimation.computeHistoricalVolatilities(historicalRentabilities, NumberOfEstimationDates);
            ParametersEstimation.computeHistoricalCorrelationMatrix(historicalRentabilities, NumberOfEstimationDates, ParametersEstimation.getHistoricalVolatilities());
        }

        private void getHistoricalPrices(Index[] indexes, int numberOfEstimationDates, double[][] historicalPrices)
        {
            DateTime endEstimationDate = DateTime.Now.AddDays(-1);
            int ind = 0;
            foreach (Index index in indexes)
            {
                DateTime currentEstimationDate = endEstimationDate;

                for (int i = numberOfEstimationDates; i >= 0; --i)
                {
                    double pastPrice = 0;
                    while (pastPrice == 0)
                    {
                        pastPrice = index.getPastPrice(currentEstimationDate);
                        currentEstimationDate = currentEstimationDate.AddDays(-1);
                    }
                    historicalPrices[ind][i] = pastPrice;
                }
                ++ind;
            }
        }

        private void transformPricesIntoRentabilities(double[][] historicalPrices, double[][] historicalRentabilities)
        {
            int ind = 0;
            foreach (double[] prices in historicalPrices)
            {
                for (int i = 1; i < prices.Length; ++i)
                    historicalRentabilities[ind][i - 1] = (prices[i] - prices[i - 1]) / prices[i - 1];
                ++ind;
            }
        }

        public double[] getHistoricalVolatilities()
        {
            return ParametersEstimation.getHistoricalVolatilities();
        }

        public double[] getHistoricalCorrelationMatrix()
        {
            return ParametersEstimation.getHistoricalCorrelationMatrix();
        }
    }
}