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
        public Index[] indexes { get; set; }
        public Eurostral eurostral { get; set; }
        public Portfolio portfolio { get; set; }

        public EurostralManagementController()
        {
            indexes = new Index[3] { new Index("Euro Stoxx 50"), new Index("SP ASX 200"), new Index("SP 500") };
            eurostral = new Eurostral();
            portfolio = new Portfolio(indexes);
        }

        // GET: Eurostral
        public ActionResult Index()
        {
            return View();
        }

        public ActionResult getEurostralPrice()
        {
            foreach (Index index in indexes)
                index.updatePastPrices();
            eurostral.getPrice(indexes);
            return PartialView("EurostralPrice", eurostral);
        }

        public ActionResult getEurostralHedging()
        {
            foreach (Index index in indexes)
                index.updatePastPrices();
            eurostral.getHedging(indexes);
            return PartialView("EurostralHedging", eurostral);
        }
    }
}