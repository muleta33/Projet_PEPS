using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using DotNet.Highstock.Enums;
using DotNet.Highstock.Helpers;
using DotNet.Highstock.Options;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class TestPLController : Controller
    {
        //
        // GET: /TestPL/
        public ActionResult Index()
        {
            ViewModelTestPL model = new ViewModelTestPL();
            return View(model);
        }

        [HttpPost]
        public ActionResult onChangeTest(ViewModelTestPL model)
        {
            List<object[]> val = model.getCurrentMode().getRandomPLData();

            List<object[]> serie1 = new List<object[]>();
            List<object[]> serie2 = new List<object[]>();

            foreach (var obj in val)
            {
                serie1.Add(new object[] { obj[0], obj[1] });
                serie2.Add(new object[] { obj[0], obj[2] });
            }

            object[] s1 = serie1.ToArray();
            object[] s2 = serie2.ToArray();

            DotNet.Highstock.Highstock chart = new DotNet.Highstock.Highstock("chart")
                .InitChart(new Chart
                {
                    Type = ChartTypes.Line,
                    ClassName = "chart"
                })
                .SetRangeSelector(new RangeSelector
                {
                    AllButtonsEnabled = true,
                    Selected = 1
                })
                .SetTitle(new Title
                {
                    Text = model.getCurrentMode().Name
                })
                .SetYAxis(new YAxis
                {
                    Title = new YAxisTitle { Text = "Valeur" }
                })
                .SetTooltip(new Tooltip
                {
                    ValueDecimals = 4
                })
                .SetSeries(new[]
                {
                    new Series { Name = "Portefeuille de couverture", Data = new Data(s1) },
                    new Series { Name = "Produit", Data = new Data(s2) }
                })
                ;

            return PartialView("PartialViewTestPL", chart);
        }

	}
}