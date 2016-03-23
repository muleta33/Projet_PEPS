using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Timers;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class PricingAndHedgingController : Controller
    {
        private const int UnderlyingNumber = 3;

        public const double InterestRate = 0.0485;

        public static Index[] Indexes = new Index[UnderlyingNumber] { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };
        public static ExchangeRate[] ExchangeRates = new ExchangeRate[UnderlyingNumber] { new ExchangeRate("eur", "usd"), new ExchangeRate("eur", "aud"), new ExchangeRate("usd", "aud") };
        public static Eurostral Eurostral = new Eurostral(Indexes, ExchangeRates, UnderlyingNumber);
        public static Portfolio Portfolio = new Portfolio(2 * UnderlyingNumber);

        public static bool IsInStartMode = true;
        public static bool IsHedgingInitialized = false;

        // GET: PricingAndHedging
        public ActionResult Index()
        {
            ViewModelPricingAndHedging modelPricingAndHedging = new ViewModelPricingAndHedging();
            return View(modelPricingAndHedging);
        }

        public ActionResult getPrice()
        {
            Eurostral.getPrice();
            return PartialView("EurostralPrice", Eurostral);
        }

        /*public void testFunction(object sender, ElapsedEventArgs e)
        {
            int a = 2;
        }*/

        [HttpPost]
        public ActionResult hedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
        {
            /*Timer timer = new Timer(modelPricingAndHedging.Frequency * 1000);
            timer.Elapsed += new ElapsedEventHandler(testFunction);
            timer.Enabled = true;*/
            if (IsInStartMode)
            {
                if (modelPricingAndHedging.isInAutomaticMode())
                    IsInStartMode = false;
                if (!IsHedgingInitialized)
                {
                    IsHedgingInitialized = true;
                    return startHedgingPortfolio(modelPricingAndHedging);
                }
                else
                    return rebalanceHedgingPortfolio(modelPricingAndHedging);
            }
            else
            {
                IsInStartMode = true;
                return PartialView("Portfolio", modelPricingAndHedging);
            }
        }

        [HttpPost]
        public ActionResult automaticHedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
        {
            return rebalanceHedgingPortfolio(modelPricingAndHedging);
        }

        public ActionResult startHedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
        {
            double[] indexesPrices = new double[UnderlyingNumber];
            int underlyingIndex = 0;
            foreach (Index index in Indexes)
            {
                indexesPrices[underlyingIndex] = index.getCurrentPrice();
                underlyingIndex++;
            }
            double currentTime = TimeManagement.convertCurrentTimeInDouble(Eurostral.BeginDate);
            modelPricingAndHedging.Portfolio.initialisation(Eurostral.getPrice(), Eurostral.getHedging(), indexesPrices, currentTime);
            return PartialView("Portfolio", modelPricingAndHedging);
        }

        public ActionResult rebalanceHedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
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
            modelPricingAndHedging.Portfolio.rebalancing(hedge, indexesPrices, InterestRate, currentTime);

            return PartialView("Portfolio", modelPricingAndHedging);
        }
    }
}