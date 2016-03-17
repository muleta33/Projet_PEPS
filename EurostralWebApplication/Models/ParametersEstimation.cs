using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

// Vérifier matrice semi def positive
namespace EurostralWebApplication.Models
{
    public class ParametersEstimation
    {
        public static double[] HistoricalVolatilities;
        public static double[] HistoricalCorrelationMatrix;

        public static double[] getHistoricalVolatilities()
        {
            return HistoricalVolatilities;
        }

        public static double[] getHistoricalCorrelationMatrix()
        {
            return HistoricalCorrelationMatrix;
        }

        public static void computeHistoricalVolatilities(double[][] historicalRentabilities)
        {
            HistoricalVolatilities = new double[historicalRentabilities.Length];

            int ind = 0;
            foreach (double[] rentabilities in historicalRentabilities)
            {
                HistoricalVolatilities[ind] = Math.Sqrt(computeVariance(rentabilities) * 52 * 5);
                ++ind;
            } 
        }

        public static void computeHistoricalCorrelationMatrix(double[][] historicalRentabilities, double[] volatilities)
        {
            HistoricalCorrelationMatrix = new double[historicalRentabilities.Length * historicalRentabilities.Length];

            for (int i = 0; i < historicalRentabilities.Length; ++i)
            {
                HistoricalCorrelationMatrix[i * historicalRentabilities.Length + i] = 1;
                for (int j = i + 1; j < historicalRentabilities.Length; ++j)
                {
                    double correlation = (computeCovariance(historicalRentabilities[i], historicalRentabilities[j]) * 52 * 5) / (volatilities[i] * volatilities[j]);
                    HistoricalCorrelationMatrix[i * historicalRentabilities.Length + j] = correlation;
                    HistoricalCorrelationMatrix[i + j * historicalRentabilities.Length] = correlation;
                }
            }
        }

        private static double computeVariance(double[] data)
        {
            double average = data.Average();
            double sumOfSquaresOfDifferences = data.Select(val => (val - average) * (val - average)).Sum();
            return sumOfSquaresOfDifferences / (data.Length - 1);
        }

        private static double computeCovariance(double[] data1, double[] data2)
        {
            double average1 = data1.Average();
            double average2 = data2.Average();

            double sumOfTimesOfDifferences = data1.Zip(data2, (first, second) => (first - average1) * (second - average2)).Sum();
            return sumOfTimesOfDifferences / (data1.Length - 1);
        }
    }
}