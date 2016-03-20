using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class EurostralManagementController : Controller
    {
        private const int UnderlyingNumber = 3;

        public const double InterestRate = 0.0485;

        public static Index[] Indexes = new Index[UnderlyingNumber] { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };
        public static ExchangeRate[] ExchangeRates = new ExchangeRate[UnderlyingNumber] { new ExchangeRate("eur", "usd"), new ExchangeRate("eur", "aud"), new ExchangeRate("usd", "aud") };
        public static Eurostral Eurostral = new Eurostral(Indexes, ExchangeRates, UnderlyingNumber);
        public static Portfolio Portfolio = new Portfolio(UnderlyingNumber);

        // GET: Eurostral
        public ActionResult Index()
        {
            return View("Index");
        }

        public ActionResult getEurostralPrice()
        {
            Eurostral.getPrice();
            return PartialView("EurostralPrice", Eurostral);
        }

        public ActionResult getIndexesPrices()
        {
            double[] indexesPrices = new double[UnderlyingNumber];
            int underlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                indexesPrices[underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }
            return PartialView("IndexesPrices", indexesPrices);
        }

        public ActionResult startHedgingEurostral()
        {
            double[] indexesPrices = new double[UnderlyingNumber];
            int underlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                indexesPrices[underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }
            double currentTime = TimeManagement.convertCurrentTimeInDouble(Eurostral.BeginDate);
            Portfolio.initialisation(Eurostral.getPrice(), Eurostral.getHedging(), indexesPrices, currentTime);
            return PartialView("Portfolio", Portfolio);
        }

        public ActionResult rebalanceHedgingPortfolio()
        {
            double[] hedge = Eurostral.getHedging();

            double[] indexesPrices = new double[UnderlyingNumber];
            int underlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                indexesPrices[underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }

            double currentTime = TimeManagement.convertCurrentTimeInDouble(Eurostral.BeginDate);
            Portfolio.rebalancing(hedge, indexesPrices, InterestRate, currentTime);

            return PartialView("Portfolio", Portfolio);
        }

    }
}