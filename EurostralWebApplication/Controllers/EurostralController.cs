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
        // GET: Eurostral
        public ActionResult Index()
        {
            Eurostral eurostral = new Eurostral();
            ViewBag.Price = eurostral.getPrice();
            return View();
        }
    }
}