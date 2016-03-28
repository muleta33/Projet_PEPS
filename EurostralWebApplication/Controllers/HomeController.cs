using EurostralWebApplication.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace EurostralWebApplication.Controllers
{
    public class HomeController : Controller
    {

        private const int UnderlyingNumber = 3;
        public static Index[] Indexes = new Index[UnderlyingNumber] { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };
        public static ExchangeRate[] ExchangeRates = new ExchangeRate[UnderlyingNumber] { new ExchangeRate("EUR", "USD"), new ExchangeRate("EUR", "AUD"), new ExchangeRate("USD", "AUD") };
        public static Eurostral Eurostral = new Eurostral(Indexes, ExchangeRates, UnderlyingNumber);

        public ActionResult Index()
        {
            return View();
        }

        public ActionResult UpdatePrices()
        {
            foreach (Index index in Eurostral.Indexes)
                index.getCurrentPrice();
            Eurostral.computeCurrentPerformances();
            foreach (ExchangeRate exchangeRate in ExchangeRates)
                exchangeRate.getCurrentValue();
            return PartialView("RealTimePrices", Eurostral);
        }
    }
}