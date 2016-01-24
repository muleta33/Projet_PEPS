using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class EurostralController : Controller
    {
        public Eurostral eurostral { get; set; }
        public Portfolio portfolio { get; set; }

        public EurostralController()
        {
            Index[] indexes = new Index[3] { new Index("Euro Stoxx 50"), new Index("SP ASX 200"), new Index("SP 500") };
            eurostral = new Eurostral(indexes);
            portfolio = new Portfolio(indexes);
        }

        // GET: Eurostral
        public ActionResult Index()
        {
            ViewBag.Price = eurostral.getPrice();
            return View(portfolio);
        }
    }
}