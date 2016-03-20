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
            return Value;
        }
    }
}