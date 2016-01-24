using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class Index
    {
        public string Name { get; set; }
        public List<double> PastPrices { get; set; }

        public Index(string name)
        {
            Name = name;
            PastPrices = new List<double>();
            initialisePastPrices();
        }

        private void initialisePastPrices()
        {
            // Récupération des prix du passé
            PastPrices.Add(150);
            PastPrices.Add(200);
        }

        public double getCurrentPrice()
        {
            // Récupérer prix
            return 170;
        }

        public void updatePastPrices()
        {
            // Mise à jour de la liste contenant les prix du passé aux dates de constatations
        }
    }
}