using System;
using System.Net;
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

        public Index(string name, string yahooFinanceName)
        {
            Name = name;
            YahooFinanceName = yahooFinanceName;
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
            return Convert.ToDouble(dataReturn.data.Ds.Tables[0].ToString().Replace(".", ",")); // Attention si erreur ???
        }

        
    }
}