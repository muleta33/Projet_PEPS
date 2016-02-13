using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class Portfolio
    {
        private const int underlyingNumber = 3; // à mettre dans un seul fichier ???
        private double LastRebalancing;

        public double[] Composition { get; set; }
        public double RiskFreeRateInvestment { get; set; }
        public double Value;

        public Portfolio()
        {
            Composition = new double[underlyingNumber];
            RiskFreeRateInvestment = 0;
            LastRebalancing = 0;
            Value = 0;
        }

        public void initialisation(double optionPrice, double[] composition, double[] prices, double currentTime)
        {
            Array.Copy(composition, Composition, underlyingNumber);
            RiskFreeRateInvestment = optionPrice - Enumerable.Range(0, underlyingNumber).Sum(i => Composition[i] * prices[i]);
            LastRebalancing = currentTime;
            Value = optionPrice;
        }

        public void rebalancing(double[] newComposition, double[] prices, double interestRate, double currentTime)
        {
            Composition = Composition.Zip(newComposition, (firstArray, secondArray) => firstArray - secondArray).ToArray();
            RiskFreeRateInvestment = RiskFreeRateInvestment * Math.Exp(interestRate * (currentTime - LastRebalancing)) + Enumerable.Range(0, underlyingNumber).Sum(i => Composition[i] * prices[i]);
            Array.Copy(newComposition, Composition, underlyingNumber);
            LastRebalancing = currentTime;
            Value = computeValue(prices);
        }

	    public double computeValue(double[] prices)
        {
            Value = RiskFreeRateInvestment + Enumerable.Range(0, underlyingNumber).Sum(i => Composition[i] * prices[i]);
            return Value;
        }

        public double computeFinalValue(double[] prices, double interestRate, double timeStep)
        {
            Value = RiskFreeRateInvestment * Math.Exp(interestRate * timeStep) + Enumerable.Range(0, underlyingNumber).Sum(i => Composition[i] * prices[i]);
            return Value;
        }
    }
}