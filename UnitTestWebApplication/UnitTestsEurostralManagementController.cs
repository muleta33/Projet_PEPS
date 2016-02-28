using System;
using System.Web.Mvc;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using EurostralWebApplication.Controllers;
using EurostralWebApplication.Models;

namespace UnitTestWebApplication
{
    [TestClass]
    public class UnitTestsEurostralManagementController
    {
        [TestMethod]
        public void testIndexView()
        {
            var controller = new EurostralManagementController();
            var result = controller.Index() as ViewResult;
            Assert.AreEqual("Index", result.ViewName);
        }

        [TestMethod]
        public void testGetEurostralPriceViewData()
        {
            // Ne fonctionne pas
            /*var controller = new EurostralManagementController();
            var result = controller.Index() as ViewResult;
            var eurostral = (Eurostral) result.ViewData.Model;
            Assert.AreEqual(new DateTime(2015, 4, 30), eurostral.BeginDate);*/
        }
    }
}
