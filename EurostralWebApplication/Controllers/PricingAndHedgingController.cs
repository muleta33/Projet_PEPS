using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class PricingAndHedgingController : Controller
    {
        private const int UnderlyingNumber = 3;

        public const double InterestRate = 0.0485;

        public static Index[] Indexes = new Index[UnderlyingNumber] { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };
        public static ExchangeRate[] ExchangeRates = new ExchangeRate[UnderlyingNumber] { new ExchangeRate("eur", "eur"), new ExchangeRate("aud", "eur"), new ExchangeRate("usd", "eur") };
        public static Eurostral Eurostral = new Eurostral(Indexes, ExchangeRates, UnderlyingNumber);

        public static ViewModelPricingAndHedging ModelPricingAndHedging = new ViewModelPricingAndHedging();

        public static bool IsInStartMode = true;
        public static bool IsHedgingInitialized = false;

        // GET: PricingAndHedging
        public ActionResult Index()
        {
            if (Session["Portfolio"] == null)
            {
                Session["Portfolio"] = new Portfolio(2 * UnderlyingNumber);
                Session.Timeout = 45;
            }
            return View(ModelPricingAndHedging);
        }

        public ActionResult getPrice()
        {
            Eurostral.getPrice();
            return PartialView("EurostralPrice", Eurostral);
        }

        [HttpPost]
        public ActionResult hedgingPortfolio(string submitButton, ViewModelPricingAndHedging modelPricingAndHedging)
        {
            // Si on est dans le cas d'un rebalancement automatique
            if (submitButton.CompareTo("Rebalancement") == 0)
                return rebalanceHedgingPortfolio(modelPricingAndHedging);
            // Sinon, c'est l'utilisateur qui envoie cette requête (rebalancement, arrêt ou lancement du mode automatique)
            else
            {
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
                    return PartialView("Portfolio", (Portfolio)Session["Portfolio"]);
                }
            }
        }

        public ActionResult startHedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
        {
            double[] prices = Eurostral.getIndexAndExchangeRateSpots();
            double currentTime = TimeManagement.convertCurrentTimeInDouble(Eurostral.BeginDate);
            ((Portfolio)Session["Portfolio"]).initialisation(Eurostral.getPrice(), Eurostral.getHedging(), prices, currentTime);
            return PartialView("Portfolio", (Portfolio)Session["Portfolio"]);
        }

        public ActionResult rebalanceHedgingPortfolio(ViewModelPricingAndHedging modelPricingAndHedging)
        {
            double[] prices = Eurostral.getIndexAndExchangeRateSpots();
            double currentTime = TimeManagement.convertCurrentTimeInDouble(Eurostral.BeginDate);
            ((Portfolio)Session["Portfolio"]).rebalancing(Eurostral.getHedging(), prices, InterestRate, currentTime);
            return PartialView("Portfolio", (Portfolio)Session["Portfolio"]);
        }
    }
}