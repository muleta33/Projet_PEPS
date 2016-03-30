using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class ExchangeRate
    {
        public string DomesticCurrency { get; set; }
        public string ForeignCurrency { get; set; }
        public double Value { get; set; }
        public double IsIncreasingOrDecreasing { get; set; }

        public ExchangeRate(string domesticCurrency, string foreignCurrency)
        {
            DomesticCurrency = domesticCurrency;
            ForeignCurrency = foreignCurrency;
            Value = 0;
            IsIncreasingOrDecreasing = 0;
        }

        public double getCurrentValue()
        {
            if (String.Compare(DomesticCurrency, ForeignCurrency, true) == 0)
                Value = 1;
            else
            {
                WebClient client = new WebClient();
                string url = "http://localhost:8080/exchange/realtime/" + DomesticCurrency + "/" + ForeignCurrency;
                var json = client.DownloadString(url);
                DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
                double value = Convert.ToDouble(dataReturn.data.Ds.Tables[0].Rows[0].ItemArray[0].ToString().Replace(".", ",")); // Attention si erreur ???
                if (value > Value)
                    IsIncreasingOrDecreasing = 1;
                else if (value < Value)
                    IsIncreasingOrDecreasing = -1;
                else
                    IsIncreasingOrDecreasing = 0;
                Value = value;
            }
            return Value;
        }

        public double getPastValue(DateTime retrievingDate)
        {
            if (String.Compare(DomesticCurrency, ForeignCurrency, true) == 0)
                return 1;

            WebClient client = new WebClient();
            bool retrieveInverseExchangeRate = false;
            // Forme de l'url : "http://localhost:8080/exchange/2015-01-01/2015-01-02/eur/usd";
            string url = "http://localhost:8080/exchange/";
            url += retrievingDate.Year + "-" + retrievingDate.Month + "-" + retrievingDate.Day + "/";
            url += retrievingDate.Year + "-" + retrievingDate.Month + "-" + retrievingDate.Day + "/";
            if (String.Compare(DomesticCurrency, "eur", true) == 0)
                url += DomesticCurrency + "/" + ForeignCurrency;
            else if (String.Compare(ForeignCurrency, "eur", true) == 0)
            {
                url += ForeignCurrency + "/" + DomesticCurrency;
                retrieveInverseExchangeRate = true;
            }
            else if (String.Compare(DomesticCurrency, "usd", true) == 0)
                url += DomesticCurrency + "/" + ForeignCurrency;
            else if (String.Compare(ForeignCurrency, "usd", true) == 0)
            {
                url += ForeignCurrency + "/" + DomesticCurrency;
                retrieveInverseExchangeRate = true;
            }
            else
                return 0;

            var json = client.DownloadString(url);
            DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
            double pastValue = 0;
            if ((dataReturn.data != null) && (dataReturn.data.Ds.Tables[0].Rows.Count > 0))
            {
                pastValue = Convert.ToDouble(dataReturn.data.Ds.Tables[0].Rows[0].ItemArray[1].ToString().Replace(".", ","));
                if (retrieveInverseExchangeRate)
                    pastValue = 1 / pastValue;
            }
            return pastValue;
        }

        public List<double> getPastValue(DateTime firstDate, DateTime lastDate)
        {
            if (String.Compare(DomesticCurrency, ForeignCurrency, true) == 0)
            {
                List<double> result = new List<double>();
                for (int i = 0; i < (lastDate - firstDate).Days - 1; ++i)
                    result.Add(1);
                return result;
            }
            WebClient client = new WebClient();
            bool retrieveInverseExchangeRate = false;
            // Forme de l'url : "http://localhost:8080/exchange/2015-01-01/2015-01-02/eur/usd";
            string url = "http://localhost:8080/exchange/";
            url += firstDate.Year + "-" + firstDate.Month + "-" + firstDate.Day + "/";
            url += lastDate.Year + "-" + lastDate.Month + "-" + lastDate.Day + "/";
            if (String.Compare(DomesticCurrency, "eur", true) == 0 || String.Compare(DomesticCurrency, "usd", true) == 0)
                url += DomesticCurrency + "/" + ForeignCurrency;
            else if (String.Compare(ForeignCurrency, "eur", true) == 0 || String.Compare(ForeignCurrency, "usd", true) == 0)
            {
                url += ForeignCurrency + "/" + DomesticCurrency;
                retrieveInverseExchangeRate = true;
            }
            else
                return new List<double>();
            var json = client.DownloadString(url);
            DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
            List<double> pastValues = new List<double>();
            if (dataReturn.data != null)
            {
                int ind = 0;
                while (ind < dataReturn.data.Ds.Tables[0].Rows.Count)
                {
                    double pastValue = Convert.ToDouble(dataReturn.data.Ds.Tables[0].Rows[ind].ItemArray[1].ToString().Replace(".", ","));
                    if (retrieveInverseExchangeRate)
                        pastValue = 1 / pastValue;
                    pastValues.Add(pastValue);
                    ++ind;
                }
            }
            return pastValues;
        }
    }
}