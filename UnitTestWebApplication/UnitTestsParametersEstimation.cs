using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using EurostralWebApplication.Models;

namespace UnitTestsWebApplication
{
    [TestClass]
    public class UnitTestsParametersEstimation
    {
        [TestMethod]
        public void computeHistoricalVolatility()
        {
            double[][] rentabilities = new double[1][];
            rentabilities[0] = new double[5];
            for (int i = 0; i < 5; ++i)
                rentabilities[0][i] = i + 1;
            ParametersEstimation.computeHistoricalVolatilities(rentabilities);
            double[] volatility = ParametersEstimation.getHistoricalVolatilities();

            Assert.AreEqual(25.4951, volatility[0], 0.0001);
        }

        [TestMethod]
        public void computeHistoricalCorrelationMatrix()
        {
            double[][] rentabilities = new double[2][];
            rentabilities[0] = new double[5];
            rentabilities[1] = new double[5];
            for (int i = 0; i < 5; ++i)
            {
                rentabilities[0][i] = i + 1;
                rentabilities[1][i] = i;
            }
            ParametersEstimation.computeHistoricalVolatilities(rentabilities);
            double[] volatilities = ParametersEstimation.getHistoricalVolatilities();
            ParametersEstimation.computeHistoricalCorrelationMatrix(rentabilities, volatilities);
            double[] correlationMatrix = ParametersEstimation.getHistoricalCorrelationMatrix();

            Assert.AreEqual(1, correlationMatrix[0]);
            Assert.AreEqual(1, correlationMatrix[1], 0.0001);
            Assert.AreEqual(1, correlationMatrix[2], 0.0001);
            Assert.AreEqual(1, correlationMatrix[3]);
        }
    }
}
