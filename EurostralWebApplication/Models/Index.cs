using System;
using System.Net;
using System.Data;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Newtonsoft.Json;

namespace EurostralWebApplication.Models
{
    public class Index
    {
        public string Name { get; set; }
        public string YahooFinanceName { get; set; }
        public double Price { get; set; }
        public double Performance { get; set; }
        public int IsIncreasingOrDecreasing { get; set; }

        public Index(string name, string yahooFinanceName)
        {
            Name = name;
            YahooFinanceName = yahooFinanceName;
            Price = 0;
            Performance = 0;
            IsIncreasingOrDecreasing = 0;
        }

        public double getPerformance(double initialPrice)
        {
            double performance = (Price - initialPrice) / initialPrice;
            if (performance > Performance)
                IsIncreasingOrDecreasing = 1;
            else if (performance < Performance)
                IsIncreasingOrDecreasing = -1;
            else
                IsIncreasingOrDecreasing = 0;
            Performance = performance;
            return Performance;
        }

        public List<object[]> getPastPricesPeriod(DateTime firstDate, DateTime lastDate)
        {
            WebClient client = new WebClient();
            // Forme de l'url : "http://localhost:8080/actif/2015-01-01/2015-01-02/close/^GSPC";
            string url = "http://localhost:8080/actif/";
            url += firstDate.Year + "-" + firstDate.Month + "-" + firstDate.Day + "/";
            url += lastDate.Year + "-" + lastDate.Month + "-" + lastDate.Day + "/";
            url += "close/";
            url += "^" + YahooFinanceName;
            var json = client.DownloadString(url);
            DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
            List<object[]> values = new List<object[]>();
            
            if (dataReturn.data != null)
            {
                DataTable data = dataReturn.data.Ds.Tables[0];
                for (int i = data.Rows.Count - 1; i >= 0; i--)
                {
                    values.Add(new object[] { Convert.ToDateTime(data.Rows[i].ItemArray[1]).ToUniversalTime(), Convert.ToDouble(data.Rows[i].ItemArray[2].ToString().Replace(".", ",")) });
                }
            }
            return values;
        }

        public double getPastPrice(DateTime retrievingDate)
        {
            WebClient client = new WebClient();
            // Forme de l'url : "http://localhost:8080/actif/2015-01-01/2015-01-02/close/^GSPC";
            string url = "http://localhost:8080/actif/";
            url += retrievingDate.Year + "-" + retrievingDate.Month + "-" + retrievingDate.Day + "/";
            url += retrievingDate.Year + "-" + retrievingDate.Month + "-" + retrievingDate.Day + "/";
            url += "close/";
            url += "^" + YahooFinanceName;
            var json = client.DownloadString(url);
            DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
            double pastPrice = 0;
            if (dataReturn.data != null)
                pastPrice = Convert.ToDouble(dataReturn.data.Ds.Tables[0].Rows[0].ItemArray[2].ToString().Replace(".", ","));
            return pastPrice;
        }

        public double getCurrentPrice()
        {
            WebClient client = new WebClient();
            string url = "http://localhost:8080/actif/realtime/E" + YahooFinanceName;
            var json = client.DownloadString(url);
            DataRetrieving.DataReturn dataReturn = JsonConvert.DeserializeObject<DataRetrieving.DataReturn>(@json);
            Price = Convert.ToDouble(dataReturn.data.Ds.Tables[0].Rows[0].ItemArray[0].ToString().Replace(".", ",")); // Attention si erreur ???
            return Price;
        }

        
    }
}