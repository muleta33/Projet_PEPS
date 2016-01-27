using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class Portfolio
    {
        public Dictionary<Index, double> Composition { get; set; }
        public double RiskFreeRateInvestment { get; set; }

        public Portfolio(Index[] indexes)
        {
            Composition = new Dictionary<Index, double>();
            foreach (Index index in indexes)
                Composition.Add(index, 0);
            RiskFreeRateInvestment = 0;
        }
    }
}